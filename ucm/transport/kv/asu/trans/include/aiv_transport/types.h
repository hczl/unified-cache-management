// SPDX-License-Identifier: Apache-2.0
// include/aiv_transport/types.h
//
// Minimal AIV transport public types required by TransProvider (shipped by
// libumc_asu_transport_provider.a, npu2cpu_rdma). Kept byte-compatible with the
// Status/StatusCode subset of UCM's asu_transport/types.h.

#pragma once

#include <string>
#include <utility>

namespace UC::ASU {

enum class StatusCode {
    OK = 0,
    INVALID_ARGUMENT,
    NOT_INITIALIZED,
    TIMEOUT,
    NOT_FOUND,
    PARTIAL_FAILED,
    CONNECTION_ERROR,
    IO_ERROR,
    BUFFER_NOT_REGISTERED,
    BUFFER_NOT_SUPPORTED,
    TASK_NOT_FOUND,
    RESOURCE_BUSY,
    UNSUPPORTED,
    IN_PROGRESS,
    INTERNAL_ERROR,
    CANCELED,

    // ASU entry status codes keep raw entry result values in the low byte.
    ASU_ENTRY_RETRY_ADVISED = 0x0100 | 0x01,
    ASU_ENTRY_NO_RETRY_ADVISED = 0x0100 | 0x02,
    ASU_ENTRY_KEY_NOT_FOUND = 0x0100 | 0x03,
    ASU_ENTRY_DATA_NOT_EXIST = 0x0100 | 0x04,
    ASU_ENTRY_DELETE_FAILED = 0x0200 | 0x01,
    ASU_ENTRY_KEY_NOT_EXIST = 0x0300 | 0x00,
    ASU_ENTRY_KEY_EXIST = 0x0300 | 0x01,

    ASU_CQE_INVALID_COMMAND_OPCODE = 0x10000 | 0x001,
    ASU_CQE_INVALID_FIELD_IN_COMMAND = 0x10000 | 0x002,
    ASU_CQE_INTERNAL_ERROR = 0x10000 | 0x006,
    ASU_CQE_WRITE_FAULT = 0x10000 | 0x280,
    ASU_CQE_UNRECOVERED_READ_ERROR = 0x10000 | 0x281,
    ASU_CQE_KEY_NOT_EXIST = 0x10000 | 0x701,
    ASU_CQE_OUT_OF_CREATE_SIZE = 0x10000 | 0x712,
    ASU_CQE_IO_TIMEOUT = 0x10000 | 0x716,
    ASU_CQE_KEY_ALREADY_EXISTED = 0x10000 | 0x723,
    ASU_CQE_RESOURCE_BUSY = 0x10000 | 0x731,
    ASU_CQE_CHECK_RESULT_BUFFER = 0x10000 | 0x732,
};

struct Status {
    StatusCode code{StatusCode::OK};
    std::string message;

    bool ok() const noexcept { return code == StatusCode::OK; }

    static Status OK() { return {}; }
    static Status Error(StatusCode c, std::string msg) { return Status{c, std::move(msg)}; }
};

}  // namespace UC::ASU
