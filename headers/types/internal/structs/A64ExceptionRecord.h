#pragma
#include <cstdint>
#include "../enums/A64ExceptionLevel.h"

namespace arm_emu {

    struct ExceptionRecord {
        Exception       exceptype;  // Exception class
        std::uint32_t   syndrome;   // Syndrome record - 25bits
        std::uint64_t   vaddress;   // Virtual fault address  - 64bits
        bool            ipavalid;   // Physical fault address for second stage faults is valid 
        bool            NS;         // Physical fault address for second stage faults is Non-secure or secure
        std::uint64_t   ipaddress;  // Physical fault address for second stage faults - 52bits

        ExceptionRecord() = default;
        ExceptionRecord(const ExceptionRecord&) = default;
        ExceptionRecord(ExceptionRecord&&) = default;
        ExceptionRecord& operator=(const ExceptionRecord&) = default;
        ExceptionRecord& operator=(ExceptionRecord&&) = default;
    };

}