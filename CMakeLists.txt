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

set(SOURCE "run_arm_emu.cpp")

################################################################################
### Add build target
################################################################################
add_executable(${PROJECT_NAME} ${SOURCE})

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