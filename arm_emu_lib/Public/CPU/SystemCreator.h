#if !defined(SYSTEMCREATOR_H_INCLUDED_B73D52A7_8950_40F2_98E8_3C8F01AF04B6)
    #define SYSTEMCREATOR_H_INCLUDED_B73D52A7_8950_40F2_98E8_3C8F01AF04B6

    #include <API/Api.h>
    #include <CPU/ICPU.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>
    #include <functional>
    #include <memory>

namespace arm_emu {

    enum class CPUType
    {
        A64,

        A32, /* Unavailable */

        T32, /* Unavailable */
    };

    struct SystemSettings {
        CPUType      CPUType;
        std::uint8_t nCores;
        std::uint8_t nThreadsPerCore;

        std::uint64_t L1CacheSize;
        std::uint64_t L2CacheSize;
        std::uint64_t L3CacheSize;

        std::uint64_t StackSize;

        std::uint64_t RamSize;
    };

    class SystemCreator {
      public:
        SystemCreator()                     = delete;
        SystemCreator(const SystemCreator&) = delete;
        SystemCreator& operator=(const SystemCreator&) = delete;
        SystemCreator(SystemCreator&&) noexcept        = delete;
        SystemCreator& operator=(SystemCreator&&) noexcept = delete;
        ~SystemCreator()                                   = delete;

        ARMEMU_API static UniqueRef< ICPU > CreateCPU(const SystemSettings& settings);
        ARMEMU_API static UniqueRef< ICPU > CreateCPU(std::string name, const SystemSettings& settings);
    };

} // namespace arm_emu

#endif // !defined(SYSTEMCREATOR_H_INCLUDED_B73D52A7_8950_40F2_98E8_3C8F01AF04B6)
