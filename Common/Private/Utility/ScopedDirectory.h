#if !defined(SCOPEDDIRECTORY_H_INCLUDED_163B6CCC_C89D_431D_BD29_F5C784B6EB13)
    #define SCOPEDDIRECTORY_H_INCLUDED_163B6CCC_C89D_431D_BD29_F5C784B6EB13

    #include <API/Api.h>
    #include <chrono>
    #include <filesystem>
    #include <format>

BEGIN_NAMESPACE

struct [[nodiscard]] ScopedDirectory {
    static constexpr int N_trials = 100;

    explicit ScopedDirectory(const std::string_view name = "directory") {
        for (auto trial = 0; trial < N_trials; ++trial) {
            const auto path =
                std::filesystem::temp_directory_path() /
                std::format("{}-{}-{:04x}", name,
                            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), trial);

            if (std::filesystem::create_directories(path)) {
                m_path = path;
                return;
            }
        }
        std::abort();
    }

    ~ScopedDirectory() {
        std::filesystem::remove_all(m_path);
    }

    const std::filesystem::path &Path() {
        return m_path;
    }

    DELETE_COPY_CLASS(ScopedDirectory)
    DELETE_MOVE_CLASS(ScopedDirectory)

  private:
    std::filesystem::path m_path;
};

END_NAMESPACE

#endif // !defined(SCOPEDDIRECTORY_H_INCLUDED_163B6CCC_C89D_431D_BD29_F5C784B6EB13)
