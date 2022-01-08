#if !defined(EVENTCATEGORY_H_INCLUDED_E598D7C2_54EA_4E17_865C_304D12C3126C)
    #define EVENTCATEGORY_H_INCLUDED_E598D7C2_54EA_4E17_865C_304D12C3126C

    #include <cstdint>

namespace arm_emu {

    enum class EventCategory : std::uint32_t
    {
        Undefined = 0x00000000,

        // Window Events
        Window = 0x00000001,

        // Input for both keyboard and mouse
        Input = 0x00000002,

        // Keyboard Events
        Keyboard = 0x00000004,

        // Mouse Events
        Mouse = 0x00000008,

        AppEvent = 0x00000010,

        CompilerEvent = 0x00000020,

        ControlEvent = 0x00000040,

        CPUEvent = 0x00000080,

        UIInputEvent = 0x00000100,

        ProgramEvent = 0x00000200,
    };

} // namespace arm_emu

#endif // !defined(EVENTCATEGORY_H_INCLUDED_E598D7C2_54EA_4E17_865C_304D12C3126C)
