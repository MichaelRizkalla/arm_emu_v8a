################################################################################
### Programming language version and compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(imgui-filedialog CXX)

################################################################################
### Includes - source files, header files, moc files
################################################################################
set(IMGUIFD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ImGuiFileDialog)
file(GLOB IMGUIFD_HEADERS "${IMGUIFD_DIR}/*.h" "${IMGUIFD_DIR}/*.hpp"
                        "${IMGUIFD_DIR}/stb/stb_image.h" 
                        "${IMGUIFD_DIR}/stb/stb_image_resize.h" 
                        "${IMGUIFD_DIR}/dirent/dirent.h")
file(GLOB IMGUIFD_SOURCES "${IMGUIFD_DIR}/*.cpp" "${IMGUIFD_DIR}/*.c")

set(IMGUIFD_INCLUDE_DIRS "${IMGUIFD_DIR}/" "${COMMON_DIR}/GLFW/include/" "${COMMON_DIR}/ImGui/")
set(IMGUIFD_SOURCE_DIRS "${IMGUIFD_DIR}/")

################################################################################
### Add build target
################################################################################
add_library(${PROJECT_NAME} STATIC ${IMGUIFD_HEADERS} ${IMGUIFD_SOURCES})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUIFD_INCLUDE_DIRS} ${IMGUIFD_SOURCE_DIRS})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic 
                                    -Wno-unknown-pragmas -Wno-unused-function -O0)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3 -fPIC)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic 
                                    -Wno-unknown-pragmas -Wno-unused-function -O0)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions -std=c++17 -Wpedantic 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3 -fPIC)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /RTC1 /diagnostics:classic
                                    /fp:precise /EHsc)
    target_link_options(${PROJECT_NAME} PRIVATE /INCREMENTAL /MACHINE:X64)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DMSVC)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    /W4 /WX- /permissive- /std:c++17
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc /Ob2 /Ox /Oi)
    target_link_options(${PROJECT_NAME} PRIVATE /INCREMENTAL /MACHINE:X64)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DMSVC)
endif()
