#ifndef SHARED_OPERATIONS_FUNCTIONS_MEMORY_HPP
#define SHARED_OPERATIONS_FUNCTIONS_MEMORY_HPP

#include <cstdint>

namespace arm_emu {

    enum class Fault : std::uint32_t
    {
        None,
        AccessFlag,
        Alignment,
        Background,
        Domain,
        Permission,
        Translation,
        AddressSize,
        SyncExternal,
        SyncExternalOnWalk,
        SyncParity,
        SyncParityOnWalk,
        AsyncParity,
        AsyncExternal,
        Debug,
        TLBConflict,
        BranchTarget,
        HWUpdateAccessFlag,
        Lockdown,
        Exclusive,
        ICacheMaint
    };

    inline static std::uint64_t LOG2_TAG_GRANULE = 4;

    inline static std::uint64_t TAG_GRANULE = static_cast< std::uint64_t >(1) << LOG2_TAG_GRANULE;

    /// <summary>
    /// Returns the effective TBI in the AArch64 stage 1 translation regime for "el".
    /// </summary>
    /// <param name="address"></param>
    /// <param name="IsInstr"></param>
    /// <param name="el"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto EffectiveTBI(std::bitset< 64 > address, bool IsInstr, std::bitset< 2 > el) noexcept {
        (void)el;
        (void)IsInstr;
        (void)address;
        /*auto regime = S1TranslationRegime(el);

        bool tbi, tbid;*/
        /*switch (regime) {
            case ExceptionLevel::EL1: {
                tbi = address[55] == true ? TCR_EL1.TBI1() : TCR_EL1.TBI0();
                if (HavePACExt()) tbid = address[55] == true ? TCR_EL1.TBID1() : TCR_EL1.TBID0();
            } break;
            case ExceptionLevel::EL2: {
                if (HaveVirtHostExt() && ELIsInHost(el)) tbi = address[55] == true ? TCR_EL2.TBI1() : TCR_EL2.TBI0();
                if (HavePACExt())
                    tbid = address[55] == true ? TCR_EL2.TBID1() : TCR_EL2.TBID0();
                else
                    tbi = TCR_EL2.TBI();
                if (HavePACExt()) tbid = TCR_EL2.TBID();
            } break;
            case ExceptionLevel::EL3: {
                tbi = TCR_EL3.TBI();
                if (HavePACExt()) tbid = TCR_EL3.TBID();
            } break;
        }*/
        /*return (tbi == true && (!HavePACExt() || tbid == '0' || !IsInstr) ? true : false); */
        return false;
    }

    /// <summary>
    /// Return the MSB number of a virtual address in the stage 1 translation regime for "el".
    /// If EL1 is using AArch64 then addresses from EL0 using AArch32 are zero-extend
    /// </summary>
    /// <param name="address"></param>
    /// <param name="IsInstr"></param>
    /// <param name="el"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto AddrTop(const std::bitset< 64 >& address, bool IsInstr, std::bitset< 2 > el) noexcept {
        /*auto regime = S1TranslationRegime(el);*/
        /*if ELUsingAArch32(regime) then
            // AArch32 translation regime.
            return 31;
        else*/
        // Always AArch64
        if (EffectiveTBI(address, IsInstr, el) == true) {
            return 55;
        } else {
            return 63;
        }
    }

    // Will be used when implementing FEAT_MTE
    /*static std::bitset<4> AllocationTagFromAddress(std::bitset<64> tagged_address) {
        std::bitset<4> return_value;
        return_value[0] = tagged_address[56];
        return_value[1] = tagged_address[57];
        return_value[2] = tagged_address[58];
        return_value[3] = tagged_address[59];
        return return_value;
    }*/

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_MEMORY_HPP
