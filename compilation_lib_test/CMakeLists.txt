################################################################################
### Project name
################################################################################
project(compilation_lib_test)

################################################################################
### Includes
################################################################################
set(DECODE_PROJ arm_emu_decodes_lib)
set(ARMEMU_LIB_PROJ arm_emu_lib)
set(COMPILATION_LIB_PROJ compilation_lib)
set(GTEST_PROJ gtest)
set(FMT_PROJ fmt::fmt-header-only)
set(DECODES_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${DECODE_PROJ}/Public/")
set(DECODES_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${DECODE_PROJ}/Private/")
set(ARMEMU_LIB_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${ARMEMU_LIB_PROJ}/Public/")
set(ARMEMU_LIB_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${ARMEMU_LIB_PROJ}/Private/")
set(COMPILATION_LIB_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${COMPILATION_LIB_PROJ}/Public/")
set(COMPILATION_LIB_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${COMPILATION_LIB_PROJ}/Private/")

set(GTEST_DIR "${COMMON_DIR}/GTest/googletest/")

set(PUBLIC_INCLUDE_DIR ${COMMON_DIR}/Public/ ${COMPILATION_LIB_PUBLIC_INCLUDE_DIR}
                        ${DECODES_PUBLIC_INCLUDE_DIR} ${ARMEMU_LIB_PUBLIC_INCLUDE_DIR})
set(PRIVATE_INCLUDE_DIR ${COMMON_DIR}/Private/ ${GTEST_DIR}/include/ ${COMPILATION_LIB_PRIVATE_INCLUDE_DIR}
                        ${DECODES_PRIVATE_INCLUDE_DIR} ${ARMEMU_LIB_PRIVATE_INCLUDE_DIR})


set(GLOB_RECURSE PUBLIC_HEADERS 
    "${COMMON_DIR}/Public/*.h"
    "${DECODES_PUBLIC_INCLUDE_DIR}/*.h"
    "${ARMEMU_LIB_PUBLIC_INCLUDE_DIR}/*.h"
    "${COMPILATION_LIB_PUBLIC_INCLUDE_DIR}/*.h"
)

file(GLOB_RECURSE LIB_FILES 
    "${DECODES_PUBLIC_INCLUDE_DIR}/*.cpp"
    "${ARMEMU_LIB_PUBLIC_INCLUDE_DIR}/*.cpp"
    "${COMPILATION_LIB_PUBLIC_INCLUDE_DIR}/*.cpp"

    "${DECODES_PRIVATE_INCLUDE_DIR}/*.*"
    "${ARMEMU_LIB_PRIVATE_INCLUDE_DIR}/*.*"
    "${COMPILATION_LIB_PRIVATE_INCLUDE_DIR}/*.*"
)

file(GLOB_RECURSE TEST_FILES 
    "Tests/*.*"
)
set(SOURCES main.cpp ${LIB_FILES} ${TEST_FILES})

################################################################################
### Add build target
################################################################################
add_executable(${PROJECT_NAME} ${SOURCES} ${LIB_FILES} ${PUBLIC_HEADERS})
add_dependencies(${PROJECT_NAME} ${GTEST_PROJ})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${DECODES_PUBLIC_INCLUDE_DIR} 
                                        ${DECODES_PRIVATE_INCLUDE_DIR}
                                        ${ARMEMU_LIB_PUBLIC_INCLUDE_DIR} 
                                        ${ARMEMU_LIB_PRIVATE_INCLUDE_DIR}
                                        ${COMPILATION_LIB_PUBLIC_INCLUDE_DIR}
                                        ${COMPILATION_LIB_PRIVATE_INCLUDE_DIR}
                                        ${PUBLIC_INCLUDE_DIR} ${PRIVATE_INCLUDE_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_TEST -DARMEMU_LOG_TYPE=ARMEMU_LOG_NONE)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GTEST_PROJ} ${FMT_PROJ})

install(TARGETS ${PROJECT_NAME} ${DECODE_PROJ} ${ARMEMU_LIB_PROJ} ${COMPILATION_LIB_PROJ}
                                    RUNTIME DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    install(FILES $<TARGET_PDB_FILE:${DECODE_PROJ}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
    install(FILES $<TARGET_PDB_FILE:${ARMEMU_LIB_PROJ}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
    install(FILES $<TARGET_PDB_FILE:${COMPILATION_LIB_PROJ}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
endif()