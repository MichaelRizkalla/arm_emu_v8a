#if !defined(COMPILATIONEVENT_H_INCLUDED_31FB4EFC_CCD6_49F9_8E12_44A95AD9A545)
    #define COMPILATIONEVENT_H_INCLUDED_31FB4EFC_CCD6_49F9_8E12_44A95AD9A545

    #include <Compiler/Compiler.h>
    #include <Event/IEvent.h>
    #include <Utility/ScopedDirectory.h>
    #include <filesystem>
    #include <vector>

namespace arm_emu {

    struct LoadCompilerEvent : public IEvent {
        LoadCompilerEvent(std::vector< std::filesystem::path > searchPaths);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        const std::vector< std::filesystem::path >& GetSearchPaths() const noexcept;

      private:
        std::vector< std::filesystem::path > m_searchPaths;
    };

    struct CompilerLoadingSuccessEvent : public IEvent {
        CompilerLoadingSuccessEvent(std::filesystem::path compilerPath);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        std::filesystem::path GetCompilerPath() const noexcept;

      private:
        std::filesystem::path m_compilerPath;
    };

    struct CompilerLoadingFailureEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

    struct CompileCodeEvent : public IEvent {
        CompileCodeEvent(std::string source, std::vector< std::filesystem::path > libraries = {});
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        std::filesystem::path                       GetFile() const noexcept;
        const std::vector< std::filesystem::path >& GetLibraries() const noexcept;
        ScopedDirectory                             MoveSourceDirectory() noexcept;

      private:
        ScopedDirectory                      m_sourceDir;
        std::vector< std::filesystem::path > m_libraries;
    };

    struct CompilationSuccessEvent : public IEvent {
        CompilationSuccessEvent(CompilationResult&& result);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        const CompilationResult& GetResults() const noexcept;
        CompilationResult&&      MoveResults() noexcept;

      private:
        CompilationResult m_results;
    };

    struct CompilationFailureEvent : public IEvent {
        CompilationFailureEvent(CompilationResult&& result);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        const CompilationResult& GetResults() const noexcept;

      private:
        CompilationResult m_results;
    };

} // namespace arm_emu

#endif // !defined(COMPILATIONEVENT_H_INCLUDED_31FB4EFC_CCD6_49F9_8E12_44A95AD9A545)
