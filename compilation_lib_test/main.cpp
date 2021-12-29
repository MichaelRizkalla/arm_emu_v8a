
#include <gtest/gtest.h>

#include <Compiler/Compiler.h>
#include <DebugUtils/Log.h>

#include <regex>
int main(int argc, char **argv) {
    using namespace arm_emu;

    Log::SetLogType(LogType::Compilation);

    auto compiler =
        Compiler::FindCompiler({ R"(C:\msys64\usr\bin\clang++.exe)", R"(C:\msys64\mingw64\bin\clang++.exe)" });

    if (!compiler.has_value()) {
        return -3;
    }

    auto &mCompiler = *compiler;
    auto  result    = mCompiler.Compile(R"(D:\Github\arm_emu_v8a\examples\prog3.cpp)", {});

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
