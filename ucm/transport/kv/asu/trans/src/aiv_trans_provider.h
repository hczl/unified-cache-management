#pragma once

#include <memory>
#include "trans_provider.h"

namespace UC::ASU {

// Authoritative declaration lives in aiv_transport/aiv_transport.h and is
// implemented inside libumc_asu_transport_provider.a (npu2cpu_rdma). It is
// re-declared here against UCM's own TransProvider so that this translation unit
// does not pull a second TransProvider definition from aiv_transport.h (ODR).
std::unique_ptr<TransProvider> CreateAIVTransProvider();

// Thin adapter that exposes the static-library AIV provider as a concrete
// TransProvider, mirroring AICPUTransProvider / FakeTransProvider so the factory
// in asu_transport_impl.cpp can construct it uniformly.
class AIVTransProvider : public TransProvider {
public:
    AIVTransProvider() : impl_(CreateAIVTransProvider()) {}

    Status CreateConnection(const std::string& localIp, const std::string& remoteIp, uint32_t port,
                            uint32_t qpNum, uint32_t timeout,
                            std::vector<ConnectionHandle>& connectionHandles) override
    {
        return impl_->CreateConnection(localIp, remoteIp, port, qpNum, timeout, connectionHandles);
    }

    std::vector<Status> DeleteConnections(
        const std::vector<ConnectionHandle>& connectionHandles) override
    {
        return impl_->DeleteConnections(connectionHandles);
    }

    std::vector<Status> Send(const std::vector<SendIoBatch>& ioBatches, uint32_t kernelCount,
                             uint32_t quietCount) override
    {
        return impl_->Send(ioBatches, kernelCount, quietCount);
    }

    Status RegisterMemory(ConnectionHandle connectionHandle,
                          const std::vector<RegisterMemoryDesc>& memoryDescs,
                          std::vector<MemHandle>& memoryHandles) override
    {
        return impl_->RegisterMemory(connectionHandle, memoryDescs, memoryHandles);
    }

    std::vector<Status> UnregisterMemory(
        const std::vector<UnregisterMemoryDesc>& memoryDescs) override
    {
        return impl_->UnregisterMemory(memoryDescs);
    }

    Status AllocThread(uint32_t threadNum, const std::vector<uint32_t>& notifyNumPerThread,
                       std::vector<ThreadHandle>& threads) override
    {
        return impl_->AllocThread(threadNum, notifyNumPerThread, threads);
    }

    std::vector<Status> FreeThread(const std::vector<ThreadHandle>& threads) override
    {
        return impl_->FreeThread(threads);
    }

    Status GetMemTokenId(MemHandle memHandle, uint32_t& tokenId) override
    {
        return impl_->GetMemTokenId(memHandle, tokenId);
    }

private:
    std::unique_ptr<TransProvider> impl_;
};

}  // namespace UC::ASU
