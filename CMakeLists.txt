################################################################################
### Minimum Cmake required
################################################################################
cmake_minimum_required(VERSION 3.19.2)

################################################################################
### Programming language version
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EMU_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Common)

set(BUILD_TESTS "Builds arm_emu_lib tests" OFF)

include_directories(${COMMON_DIR}/Private/ ${COMMON_DIR}/Public/ ${COMMON_DIR}/Boost/ ${COMMON_DIR}/Format/include/)

################################################################################
### Compiler flags
################################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -fexceptions 
                        -Wno-unknown-pragmas 
                        -Wno-1unused-function -O3 -fPIC)
    add_link_options(-s -fPIC)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -fexceptions 
                        -Wno-unknown-pragmas 
                        -Wno-unused-function -O3 -fPIC)
    add_link_options(-s -fPIC)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    string(REGEX REPLACE "/W[3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    add_compile_options(/WX- /permissive-
                        /TP /Gd /Gm- /diagnostics:classic
                        /fp:precise /EHsc)
    add_link_options(/INCREMENTAL /MACHINE:X64)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    string(REGEX REPLACE "/W[3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    add_compile_options(/WX- /permissive-
                        /TP /Gd /Gm- /diagnostics:classic
                        /fp:precise /EHsc /Ob2 /Ox /Oi)
    add_link_options(/INCREMENTAL /MACHINE:X64)
endif()

################################################################################
### Projects
################################################################################
project(ARM_EMU_PROJ)
add_subdirectory(Common/Format)
add_subdirectory(arm_emu_decodes_lib)
add_subdirectory(arm_emu_lib)
add_subdirectory(compilation_lib)

if (BUILD_TESTS)
    set(BUILD_SHARED_LIBS OFF)
    set(BUILD_GMOCK OFF)
    set(INSTALL_GTEST OFF)
    add_subdirectory(Common/GTest)
    add_subdirectory(arm_emu_lib_test)
    add_subdirectory(compilation_lib_test)
endif()
