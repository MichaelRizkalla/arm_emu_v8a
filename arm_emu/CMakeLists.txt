################################################################################
### Project name
################################################################################
project(arm_emu)

################################################################################
### Includes
################################################################################
set(ARMEMU_DECODES_LIB arm_emu_decodes_lib)
set(ARMEMU_LIB_PROJ arm_emu_lib)
set(COMPILATION_PROJ compilation_lib)

set(DECODES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../arm_emu_decodes_lib/Public")
set(ARMEMU_LIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../arm_emu_lib/Public")
set(COMPILATION_LIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../compilation_lib/Public")

set(PUBLIC_INCLUDE_DIR ${ARMEMU_LIB_INCLUDE_DIR}/ ${COMPILATION_LIB_INCLUDE_DIR}/ 
                        ${DECODES_INCLUDE_DIR}/ "Public/"
                        ${CMAKE_CURRENT_SOURCE_DIR}/Public/
                        ${COMMON_DIR}/Public/)
set(PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Private/ 
                        ${COMMON_DIR}/Private/ "Private/"
                        ${COMMON_DIR}/ImGui/
                        ${COMMON_DIR}/ImGuiFileDialog/)

set(GLOB_RECURSE PUBLIC_HEADERS 
    ${DECODES_INCLUDE_DIR}/*.h
    ${ARMEMU_LIB_INCLUDE_DIR}/*.h
    ${COMPILATION_LIB_INCLUDE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Public/*.h
    "Public/*.h")

file(GLOB_RECURSE SOURCE_FILES 
    "Private/*.*"
    "Public/*.cpp"
)

################################################################################
### Add build target
################################################################################
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PUBLIC_HEADERS})
add_dependencies(${PROJECT_NAME} ${ARMEMU_LIB_PROJ} ${COMPILATION_PROJ} 
                                    ${ARMEMU_DECODES_LIB} imgui glfw fmt imgui-filedialog)

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${PRIVATE_INCLUDE_DIR} ${PUBLIC_INCLUDE_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE)

target_link_libraries(${PROJECT_NAME} PRIVATE ${ARMEMU_LIB_PROJ} ${COMPILATION_PROJ} 
                                        ${ARMEMU_DECODES_LIB} imgui glfw fmt imgui-filedialog)

install(TARGETS ${PROJECT_NAME} ${ARMEMU_LIB_PROJ} ${COMPILATION_PROJ} 
                                        ${ARMEMU_DECODES_LIB}
    RUNTIME DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/
    LIBRARY DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
    install(FILES $<TARGET_PDB_FILE:${ARMEMU_LIB_PROJ}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
    install(FILES $<TARGET_PDB_FILE:${COMPILATION_PROJ}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
    install(FILES $<TARGET_PDB_FILE:${ARMEMU_DECODES_LIB}> DESTINATION ${ARMEMU_INSTALL_DIR}/${PROJECT_NAME}/bin/)
endif()
