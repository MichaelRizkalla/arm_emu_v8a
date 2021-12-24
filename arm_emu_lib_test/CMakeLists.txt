################################################################################
### Project name
################################################################################
set(CMAKE_CXX_STANDARD 23)
project(arm_emu_lib_test)

################################################################################
### Includes
################################################################################
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../arm_emu_lib")
set(INCLUDE_DIRS ${LIB_DIR}/Public/ ${LIB_DIR}/Private/ ${LIB_DIR}/Boost/)
file(GLOB_RECURSE LIB_FILES 
    "${LIB_DIR}/Private/*.*"
    "${LIB_DIR}/Public/*.*"
)
set(SOURCES arm_emu_lib_test.cpp ${LIB_FILES})

################################################################################
### Add build target
################################################################################
add_executable(${PROJECT_NAME} ${SOURCES})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_TEST)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    -Wall -fexceptions 
                                    -Wno-unknown-pragmas -Wno-unused-function -O3)
    target_link_options(${PROJECT_NAME} PRIVATE -s)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    string(REGEX REPLACE "/W[3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    /WX- /permissive-
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc)
    target_link_options(${PROJECT_NAME} PRIVATE /INCREMENTAL /MACHINE:X64)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
    string(REGEX REPLACE "/W[3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    target_compile_options(${PROJECT_NAME} PRIVATE 
                                    /WX- /permissive-
                                    /TP /Gd /Gm- /diagnostics:classic
                                    /fp:precise /EHsc /Ob2 /Ox /Oi)
    target_link_options(${PROJECT_NAME} PRIVATE /INCREMENTAL /MACHINE:X64)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${EMU_INSTALL_DIR}/)
