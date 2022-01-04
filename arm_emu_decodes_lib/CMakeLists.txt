################################################################################
### Project name
################################################################################
project(arm_emu_decodes_lib)

################################################################################
### Includes
################################################################################
set(PRIVATE_INCLUDR_DIRS ${COMMON_DIR}/Private/ "Private/")
set(PUBLIC_INCLUDE_DIRS ${COMMON_DIR}/Public/ "Public/")

set(GLOB_RECURSE PUBLIC_HEADERS 
    "Public/*.h"
    "${COMMON_DIR}/Public/*.h")

file(GLOB_RECURSE LIB_FILES 
    "Private/*.*"
    "Public/*.cpp"
)

################################################################################
### Add build target
################################################################################
add_library(${PROJECT_NAME} SHARED ${LIB_FILES})
add_dependencies(${PROJECT_NAME} fmt)

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${PRIVATE_INCLUDR_DIRS} ${PUBLIC_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_DLL -DARMEMU_LOG_TYPE=ARMEMU_LOG_NONE)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/
    LIBRARY DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/
    ARCHIVE DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/lib/
)
install(FILES ${PUBLIC_HEADERS} DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/include)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
endif()
