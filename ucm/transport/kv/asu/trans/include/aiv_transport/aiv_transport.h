// SPDX-License-Identifier: Apache-2.0
// include/aiv_transport/aiv_transport.h
//
// Public AIV transport interface shipped by libumc_asu_transport_provider.a
// (npu2cpu_rdma), compatible with unified-cache-management's TransProvider
// contract.
//
// It declares the abstract TransProvider contract plus the
// CreateAIVTransProvider() factory implemented inside the static library. The
// TransProvider class name is intentionally kept (it is the shared contract with
// UCM); the AIV identity lives on the concrete subclass and the
// CreateAIVTransProvider() factory.
//
// NOTE (ODR): the TransProvider class below is byte-for-byte identical to UCM's
// own ucm/transport/kv/asu/trans/src/trans_provider.h. Do NOT include both this
// header and UCM's trans_provider.h in the same translation unit, or the class
// will be defined twice. The src-side adapter (src/aiv_trans_provider.h) only
// pulls in the CreateAIVTransProvider() declaration to avoid this clash.

#pragma once

#include "aiv_transport/types.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace UC::ASU {

class TransProvider {
public:
    using ConnectionHandle = void*;
    using ThreadHandle = void*;
    using MemHandle = void*;

    virtual ~TransProvider() = default;

    virtual Status CreateConnection(const std::string& localIp, const std::string& remoteIp,
                                    uint32_t port, uint32_t qpNum, uint32_t timeout,
                                    std::vector<ConnectionHandle>& connectionHandles) = 0;

    virtual std::vector<Status> DeleteConnections(
        const std::vector<ConnectionHandle>& connectionHandles) = 0;

    struct SendIoBatch {
        ConnectionHandle connectionHandle;
        void* sendBuffer;
        void* flagBuffer;
        uint64_t len;
    };

    virtual std::vector<Status> Send(const std::vector<SendIoBatch>& ioBatches,
                                     uint32_t kernelCount, uint32_t quietCount) = 0;

    enum class MemType { MEM_DEVICE, MEM_HOST };

    struct RegisterMemoryDesc {
        MemType memoryType;
        uintptr_t addr;
        size_t size;
    };

    virtual Status RegisterMemory(ConnectionHandle connectionHandle,
                                  const std::vector<RegisterMemoryDesc>& memoryDescs,
                                  std::vector<MemHandle>& memoryHandles) = 0;

    struct UnregisterMemoryDesc {
        ConnectionHandle connectionHandle;
        MemHandle memoryHandle;
    };

    virtual std::vector<Status> UnregisterMemory(
        const std::vector<UnregisterMemoryDesc>& memoryDescs) = 0;

    virtual Status AllocThread(uint32_t threadNum, const std::vector<uint32_t>& notifyNumPerThread,
                               std::vector<ThreadHandle>& threads) = 0;

    virtual std::vector<Status> FreeThread(const std::vector<ThreadHandle>& threads) = 0;

    virtual Status GetMemTokenId(MemHandle memHandle, uint32_t& tokenId) = 0;
};

// Factory implemented by libumc_asu_transport_provider.a. Returns a ready-to-use
// AIV TransProvider (NPU AIV directly driving UDMA over the UB network).
std::unique_ptr<TransProvider> CreateAIVTransProvider();

}  // namespace UC::ASU
