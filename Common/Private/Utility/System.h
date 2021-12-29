#if !defined(SYSTEM_H_INCLUDED_42AD7DE8_3B65_4BCE_A171_4A94C0505A1F)
    #define SYSTEM_H_INCLUDED_42AD7DE8_3B65_4BCE_A171_4A94C0505A1F

    #include <API/Api.h>
    #include <Utility/File.h>
    #include <Utility/ScopedDirectory.h>
    #include <cstdlib>
    #include <string>
    #include <tuple>

BEGIN_NAMESPACE

struct System {
    STATIC_CLASS(System)

    using ResultCode    = int;
    using StdOutMessage = std::string;
    using StdErrMessage = std::string;

    static std::tuple< ResultCode, StdOutMessage, StdErrMessage > Call(const std::string &command) {
        ScopedDirectory directory {};

        const auto stdoutPath { directory.Path() / "stdout" };
        const auto stderrPath { directory.Path() / "stderr" };

        const auto submitCommand = [](const std::string &cmd, const std::filesystem::path &out,
                                      const std::filesystem::path &err) {
            return std::format(R"({} 1>"{}" 2>"{}")", cmd, out.string(), err.string());
        };

        const auto result = std::system(submitCommand(command, stdoutPath, stderrPath).c_str());

        const auto out = FileUtility::Read(stdoutPath);
        const auto err = FileUtility::Read(stderrPath);

        return { result, std::string { out.begin(), out.end() }, std::string { err.begin(), err.end() } };
    }
};

END_NAMESPACE

#endif // !defined(SYSTEM_H_INCLUDED_42AD7DE8_3B65_4BCE_A171_4A94C0505A1F)
