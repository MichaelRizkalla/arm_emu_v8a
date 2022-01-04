################################################################################
### Programming language version and compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(imgui CXX)

################################################################################
### Includes - source files, header files, moc files
################################################################################
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ImGui)
file(GLOB IMGUI_HEADERS "${IMGUI_DIR}/*.h" "${IMGUI_DIR}/*.hpp"
                        "${IMGUI_DIR}/backends/imgui_impl_glfw.h" 
                        "${IMGUI_DIR}/backends/imgui_impl_opengl3.h")
file(GLOB IMGUI_SOURCES "${IMGUI_DIR}/*.cpp" "${IMGUI_DIR}/*.c"
                        "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp" 
                        "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp")

set(IMGUI_INCLUDE_DIRS "${IMGUI_DIR}/" "${COMMON_DIR}/GLFW/include/")
set(IMGUI_SOURCE_DIRS "${IMGUI_DIR}/")

################################################################################
### Add build target
################################################################################
add_library(${PROJECT_NAME} STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_INCLUDE_DIRS} ${IMGUI_SOURCE_DIRS})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O0)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3 -fPIC)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O0)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic -Werror 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3 -fPIC)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /RTC1 /diagnostics:classic
                                    /fp:precise /EHsc)
    target_link_options(${PROJECT_NAME} PRIVATE /INCREMENTAL /MACHINE:X64)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc /Ob2 /Ox /Oi)
    target_link_options(${PROJECT_NAME} PRIVATE /INCREMENTAL /MACHINE:X64)
endif()
