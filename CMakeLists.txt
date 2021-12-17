################################################################################
### Minimum Cmake required
################################################################################
cmake_minimum_required(VERSION 3.19.2)

################################################################################
### Programming language version and compiler flags
################################################################################
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EMU_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

################################################################################
### Projects
################################################################################
project(ARM_EMU_PROJ)
add_subdirectory(arm_emu_lib)
add_subdirectory(arm_emu_lib_test)