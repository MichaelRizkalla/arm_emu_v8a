#if !defined(EVENTTYPE_H_INCLUDED_4435519C_7CF4_4D67_BC6F_A9314E8BCF0C)
    #define EVENTTYPE_H_INCLUDED_4435519C_7CF4_4D67_BC6F_A9314E8BCF0C

    #include <cstdint>

namespace arm_emu {

    enum class EventType : std::uint32_t
    {
        Undefined,

        WindowClosed,
        WindowMinimized,
        WindowResized,
        WindowFocused,
        WindowLost,
        WindowMoved,

        KeyPressed,
        KeyReleased,
        KeyCharacter,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrollTriggered,
        MouseMoved,

        // App Specific events
        ClearInput,
        SaveInput,
        LoadInputFromFile,
        Terminate,

        LoadCompiler,
        CompilerLoadingSuccessful,
        CompilerLoadingFailure,
        CompileCode,
        CompilationSuccessful,
        CompilationFailure,

        CreateCPU,
        CPUCreationSuccessful,
        CPUCreationFailure,

        LoadProgram,
        LoadProgramSuccessful,
        LoadProgramFailure,
        StepInProgramAvailable,
        StepInProgram,
        StepInProgramFinished,

        ShowProgramControls,
        HideProgramControls,
    };

} // namespace arm_emu

#endif // !defined(EVENTTYPE_H_INCLUDED_4435519C_7CF4_4D67_BC6F_A9314E8BCF0C)
