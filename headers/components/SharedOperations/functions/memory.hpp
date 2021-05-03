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
    
    inline static std::uint64_t TAG_GRANULE      = 1 << LOG2_TAG_GRANULE;

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_MEMORY_HPP
