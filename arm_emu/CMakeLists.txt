################################################################################
### Project name
################################################################################
project(arm_emu)

################################################################################
### Includes
################################################################################
set(ARMEMU_LIB_PROJ arm_emu_lib)
set(COMPILATION_PROJ compilation_lib)

set(DECODE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../arm_emu_decodes_lib/Public")
set(ARMEMU_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../arm_emu_lib/Public")
set(COMPILATION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../compilation_lib/Public")

set(INCLUDE_DIRS ${ARMEMU_LIB_DIR} ${COMPILATION_DIR} ${DECODE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/Private/ 
                ${CMAKE_CURRENT_SOURCE_DIR}/Public/
                ${COMMON_DIR}/Public/ ${COMMON_DIR}/Private/)

file(GLOB_RECURSE SOURCE_FILES 
    "Private/*.*"
    "Public/*.*"
    "${COMMON_DIR}/Public/*.*"
)

################################################################################
### Add build target
################################################################################
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} ${ARMEMU_LIB_PROJ} ${COMPILATION_PROJ})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_compile_definitions(${PROJECT_NAME} PRIVATE)

target_link_libraries(${PROJECT_NAME} PRIVATE ${ARMEMU_LIB_PROJ} ${COMPILATION_PROJ})

install(TARGETS ${PROJECT_NAME} DESTINATION ${EMU_INSTALL_DIR}/)
