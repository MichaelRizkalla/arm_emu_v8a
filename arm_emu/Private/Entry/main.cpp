
#include <API/Api.h>

#ifdef ARMEMU_OS_WINDOWS
    #include <Windows.h>
#elif defined(ARMEMU_OS_LINUX)

#endif

#include <Application/Application.h>
#include <iostream>

void HideConsole() noexcept {
#ifdef ARMEMU_OS_WINDOWS
    // ShowWindow(GetConsoleWindow(), SW_HIDE);
#elif defined(ARMEMU_OS_LINUX)

#endif
}

int main() {
    HideConsole();
    using namespace arm_emu;

    auto& app = Application::GetInstance();
    try {
        app.Run();
    } catch (failed_to_create_window& e) {
        std::cout << e.what() << '\n';
    } catch (std::exception& e) {
        std::cout << "Failed with exception: " << e.what() << '\n';
    }
}
