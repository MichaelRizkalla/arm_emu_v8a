################################################################################
### Project name
################################################################################
project(arm_emu_lib)

################################################################################
### Includes
################################################################################
set(DECODE_PROJ arm_emu_decodes_lib)
set(DECODES_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../${DECODE_PROJ}/Private/" 
                    "${CMAKE_CURRENT_SOURCE_DIR}/../${DECODE_PROJ}/Public/")
set(INCLUDE_DIRS ${COMMON_DIR}/Private/ "Private/" "Public/")

file(GLOB_RECURSE LIB_FILES 
    "Private/*.*"
    "Public/*.*"
    "${COMMON_DIR}/Private/*.*"
    "${COMMON_DIR}/Public/*.*"
)

################################################################################
### Add build target
################################################################################
add_library(${PROJECT_NAME} SHARED ${LIB_FILES} ${INCLUDE_DIRS})
add_dependencies(${PROJECT_NAME} ${DECODE_PROJ})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS} ${DECODES_INCLUDE})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_DLL)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DECODE_PROJ} fmt::fmt-header-only)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${EMU_INSTALL_DIR}/)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${EMU_INSTALL_DIR}/)
endif()
