################################################################################
### Project name
################################################################################
project(arm_emu_decodes_lib)

################################################################################
### Includes
################################################################################
set(INCLUDE_DIRS "Private/" "Public/")

file(GLOB_RECURSE LIB_FILES 
    "Private/*.*"
    "Public/*.*"
    "${COMMON_DIR}/Public/*.*"
)

################################################################################
### Add build target
################################################################################
add_library(${PROJECT_NAME} SHARED ${LIB_FILES} ${INCLUDE_DIRS})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_DLL -DARMEMU_LOG_TYPE=ARMEMU_LOG_NONE)

install(TARGETS ${PROJECT_NAME} LIBRARY RUNTIME DESTINATION ${EMU_INSTALL_DIR}/)
