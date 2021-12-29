#if !defined(SYSTEMSETTINGS_H_INCLUDED_116A820E_D1BA_40EA_822A_E4814B30020B)
    #define SYSTEMSETTINGS_H_INCLUDED_116A820E_D1BA_40EA_822A_E4814B30020B

    #include <API/Api.h>
    #include <CPU/CPUType.h>

namespace arm_emu {

    struct SystemSettings {
        alignas(8) CPUType cpuType;
        alignas(8) std::uint8_t nCores;
        alignas(8) std::uint8_t nThreadsPerCore;

        alignas(64) std::uint64_t L1CacheSize;
        alignas(64) std::uint64_t L2CacheSize;
        alignas(64) std::uint64_t L3CacheSize;

        alignas(64) std::uint64_t RamSize;

        /* Per process allocated memory */
        alignas(64) std::uint64_t StackSize;
        alignas(64) std::uint64_t HeapSize;
    };

} // namespace arm_emu

#endif // !defined(SYSTEMSETTINGS_H_INCLUDED_116A820E_D1BA_40EA_822A_E4814B30020B)
