#if !defined(SYSTEMCREATOR_H_INCLUDED_B73D52A7_8950_40F2_98E8_3C8F01AF04B6)
    #define SYSTEMCREATOR_H_INCLUDED_B73D52A7_8950_40F2_98E8_3C8F01AF04B6

    #include <API/Api.h>
    #include <CPU/ICPU.h>
    #include <CPU/SystemSettings.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>
    #include <exception>
    #include <functional>
    #include <memory>

namespace arm_emu {

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
