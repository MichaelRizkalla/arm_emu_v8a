
#include <API/Api.h>

#include <Application/Application.h>
#include <iostream>

namespace {

    void HideWindowsConsole() {
#ifdef ARMEMU_OS_WINDOWS
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif // ARMEMU_OS_WINDOWS
    }

} // namespace

int main() {
    HideWindowsConsole();
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
