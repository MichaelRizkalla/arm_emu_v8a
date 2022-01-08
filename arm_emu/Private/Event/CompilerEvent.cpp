
#include <API/HiddenAPI.h>
#include <Event/CompilerEvent.h>

#include <fstream>

BEGIN_NAMESPACE

LoadCompilerEvent::LoadCompilerEvent(std::vector< std::filesystem::path > searchPaths) : m_searchPaths(searchPaths) {
}

EventType LoadCompilerEvent::GetEventType() const noexcept {
    return EventType::LoadCompiler;
}

std::underlying_type_t< EventCategory > LoadCompilerEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CompilerEvent);
}

const std::vector< std::filesystem::path >& LoadCompilerEvent::GetSearchPaths() const noexcept {
    return m_searchPaths;
}

CompilerLoadingSuccessEvent::CompilerLoadingSuccessEvent(std::filesystem::path compilerPath) :
    m_compilerPath(compilerPath) {
}

EventType CompilerLoadingSuccessEvent::GetEventType() const noexcept {
    return EventType::CompilerLoadingSuccessful;
}

std::underlying_type_t< EventCategory > CompilerLoadingSuccessEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CompilerEvent);
}

std::filesystem::path CompilerLoadingSuccessEvent::GetCompilerPath() const noexcept {
    return m_compilerPath;
}

EventType CompilerLoadingFailureEvent::GetEventType() const noexcept {
    return EventType::CompilerLoadingFailure;
}

std::underlying_type_t< EventCategory > CompilerLoadingFailureEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CompilerEvent);
}

CompileCodeEvent::CompileCodeEvent(std::string source, std::vector< std::filesystem::path > libraries) :
    m_sourceDir(), m_libraries(std::move(libraries)) {
    const auto cppFile = m_sourceDir.Path() / "src.cpp";
    if (std::ofstream os(cppFile, std::ios::out | std::ios::binary); os.good()) {
        os.write(source.data(), static_cast< std::streamsize >(std::strlen(source.c_str())));
        os.flush();
    }
}

EventType CompileCodeEvent::GetEventType() const noexcept {
    return EventType::CompileCode;
}

std::underlying_type_t< EventCategory > CompileCodeEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CompilerEvent);
}

std::filesystem::path CompileCodeEvent::GetFile() const noexcept {
    return m_sourceDir.Path() / "src.cpp";
}

const std::vector< std::filesystem::path >& CompileCodeEvent::GetLibraries() const noexcept {
    return m_libraries;
}

ScopedDirectory CompileCodeEvent::MoveSourceDirectory() noexcept {
    return std::move(m_sourceDir);
}

CompilationSuccessEvent::CompilationSuccessEvent(CompilationResult&& result) : m_results(std::move(result)) {
}

EventType CompilationSuccessEvent::GetEventType() const noexcept {
    return EventType::CompilationSuccessful;
}

std::underlying_type_t< EventCategory > CompilationSuccessEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CompilerEvent);
}

const CompilationResult& CompilationSuccessEvent::GetResults() const noexcept {
    return m_results;
}

CompilationResult&& CompilationSuccessEvent::MoveResults() noexcept {
    return std::move(m_results);
}

CompilationFailureEvent::CompilationFailureEvent(CompilationResult&& result) : m_results(std::move(result)) {
}

EventType CompilationFailureEvent::GetEventType() const noexcept {
    return EventType::CompilationFailure;
}

std::underlying_type_t< EventCategory > CompilationFailureEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CompilerEvent);
}

const CompilationResult& CompilationFailureEvent::GetResults() const noexcept {
    return m_results;
}

END_NAMESPACE
