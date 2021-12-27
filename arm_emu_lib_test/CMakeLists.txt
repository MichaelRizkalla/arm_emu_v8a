################################################################################
### Project name
################################################################################
project(arm_emu_lib_test)

################################################################################
### Includes
################################################################################
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../arm_emu_lib")
set(GTEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/GTest/googletest/")
set(INCLUDE_DIRS ${LIB_DIR}/Public/ ${LIB_DIR}/Private/ ${LIB_DIR}/Boost/ ${GTEST_DIR}/include/ ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE LIB_FILES 
    "${LIB_DIR}/Private/*.*"
    "${LIB_DIR}/Public/*.*"
)
file(GLOB_RECURSE TEST_FILES 
    "Tests/*.*"
)
set(SOURCES main.cpp ${LIB_FILES} ${TEST_FILES})

################################################################################
### Add build target
################################################################################
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} gtest)

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_TEST -DARMEMU_LOG_TYPE=ARMEMU_LOG_NONE)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${EMU_INSTALL_DIR}/)
