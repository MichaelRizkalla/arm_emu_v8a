#ifndef SHARED_OPERATIONS_FUNCTIONS_ABORTS_HPP
#define SHARED_OPERATIONS_FUNCTIONS_ABORTS_HPP

#include <bitset>
#include <cassert>
#include "memory.hpp"
#include "system.hpp"

namespace arm_emu {

    // EncodeLDFSC()
    // Function that gives the long-descriptor FSC code for types of Fault
    std::bitset< 6 > EncodeLDFSC(Fault statuscode, int level) {
        std::bitset< 6 > result { 0b000000 };
        switch (statuscode) {
            case Fault::AddressSize:
                if (!(level <= 3 && level >= 0)) {
                    Unreachable();
                }
                result = level;
                break;
            case Fault::AccessFlag:
                if (!(level <= 3 && level >= 1)) {
                    Unreachable();
                }
                result = level;
                result |= 0b001000;
                break;
            case Fault::Permission:
                if (!(level <= 3 && level >= 1)) {
                    Unreachable();
                }
                result = level;
                result |= 0b001100;
                break;
            case Fault::Translation:
                if (!(level <= 3 && level >= 0)) {
                    Unreachable();
                }
                result = level;
                result |= 0b000100;
                break;
            case Fault::SyncExternal:
                result = 0b010000;
                break;
            case Fault::SyncExternalOnWalk:
                if (!(level <= 3 && level >= 0)) {
                    Unreachable();
                }
                result = level;
                result |= 0b010100;
                break;
            case Fault::SyncParity:
                result = 0b011000;
                break;
            case Fault::SyncParityOnWalk:
                if (!(level <= 3 && level >= 0)) {
                    Unreachable();
                }
                result = level;
                result |= 0b011100;
                break;
            case Fault::AsyncParity:
                result = 0b011001;
                break;
            case Fault::AsyncExternal:
                result = 0b010001;
                break;
            case Fault::Alignment:
                result = 0b100001;
                break;
            case Fault::Debug:
                result = 0b100010;
                break;
            case Fault::TLBConflict:
                result = 0b110000;
                break;
            case Fault::HWUpdateAccessFlag:
                result = 0b110001;
                break;
            case Fault::Lockdown:
                result = 0b110100; // IMPLEMENTATION DEFINED
                break;
            case Fault::Exclusive:
                result = 0b110101; // IMPLEMENTATION DEFINED
                break;
            default:
                Unreachable();
        }
        return result;
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_ABORTS_HPP
