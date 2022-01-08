################################################################################
### Project name
################################################################################
project(compilation_lib)

################################################################################
### Includes
################################################################################
set(DECODE_PROJ arm_emu_decodes_lib)
set(DECODES_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${DECODE_PROJ}/Public/")
set(DECODES_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${DECODE_PROJ}/Private/")

set(PUBLIC_INCLUDE_DIR ${COMMON_DIR}/Public/ "Public/")
set(PRIVATE_INCLUDE_DIR ${COMMON_DIR}/Private/ "Private/")

set(GLOB_RECURSE PUBLIC_HEADERS 
    "Public/*.h"
    "${COMMON_DIR}/Public/*.h"
    "${DECODES_PUBLIC_INCLUDE_DIR}/*.h")

file(GLOB_RECURSE LIB_FILES 
    "Private/*.*"
    "Public/*.cpp"
)

################################################################################
### Add build target
################################################################################
add_library(${PROJECT_NAME} SHARED ${LIB_FILES})
add_dependencies(${PROJECT_NAME} ${DECODE_PROJ})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${DECODES_PUBLIC_INCLUDE_DIR} 
                                        ${DECODES_PRIVATE_INCLUDE_DIR}
                                        ${PUBLIC_INCLUDE_DIR} ${PRIVATE_INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_DLL)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_link_libraries(${PROJECT_NAME} PRIVATE ${DECODE_PROJ} fmt::fmt-header-only)

install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/
    LIBRARY DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/
    ARCHIVE DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/lib/
)
install(FILES ${PUBLIC_HEADERS} DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/include/)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
endif()
