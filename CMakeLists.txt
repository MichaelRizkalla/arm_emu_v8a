################################################################################
### Minimum Cmake required
################################################################################
cmake_minimum_required(VERSION 3.13.0)
project(ARM_EMU)

################################################################################
### Programming language version and compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 20)

################################################################################
### Includes
################################################################################
set(INCLUDE_DIRS 
                    "headers/components"
                    "headers/instructions"
                    "headers/shared"
                    "headers/system"
                    "headers/types" )                 
file(GLOB_RECURSE EMU_COMPONTENTS 
    "headers/components/*.h" 
    "headers/components/*.hpp"
    "headers/components/*.cpp")
file(GLOB_RECURSE EMU_INSTRUCTIONS 
    "headers/instructions/*.h" 
    "headers/instructions/*.hpp"
    "headers/instructions/*.cpp")
file(GLOB_RECURSE EMU_SHARED 
    "headers/shared/*.h" 
    "headers/shared/*.hpp"
    "headers/shared/*.cpp")
file(GLOB_RECURSE EMU_SYSTEM 
    "headers/system/*.h" 
    "headers/system/*.hpp"
    "headers/system/*.cpp")
file(GLOB_RECURSE EMU_TYPES 
    "headers/types/*.h" 
    "headers/types/*.hpp"
    "headers/types/*.cpp")

set(SOURCE "run_arm_emu.cpp")

################################################################################
### Add build target
################################################################################
add_executable(${PROJECT_NAME} ${SOURCE} ${EMU_COMPONTENTS} 
                                        ${EMU_INSTRUCTIONS} ${EMU_SHARED}
                                        ${EMU_SYSTEM} ${EMU_TYPES})

################################################################################
### Visual studio filters
################################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    source_group("arm_emu/components" FILES ${EMU_COMPONTENTS})
    source_group("arm_emu/instructions" FILES ${EMU_INSTRUCTIONS})
    source_group("arm_emu/shared" FILES ${EMU_SHARED})
    source_group("arm_emu/system" FILES ${EMU_SYSTEM})
    source_group("arm_emu/types" FILES ${EMU_TYPES})
endif()

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PUBLIC 
                                    -Wall -fexceptions -std=c++2a 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(${PROJECT_NAME} PUBLIC -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PUBLIC 
                                    -Wall -fexceptions -std=c++2a 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(${PROJECT_NAME} PUBLIC -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(${PROJECT_NAME} PUBLIC 
                                    /W4 /WX- /permissive- /std:c++latest
                                    /TP /Gd /Gm- /RTC0 /diagnostics:classic
                                    /fp:precise /EHsc)
    target_link_options(${PROJECT_NAME} PUBLIC /INCREMENTAL /MACHINE:X64)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PUBLIC 
                                    /W4 /WX- /permissive- /std:c++latest
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc /Ob2 /Ox /Oi)
    target_link_options(${PROJECT_NAME} PUBLIC /INCREMENTAL /MACHINE:X64)
endif()