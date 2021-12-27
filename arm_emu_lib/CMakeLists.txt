################################################################################
### Project name
################################################################################
project(arm_emu_lib)

################################################################################
### Includes
################################################################################
set(INCLUDE_DIRS "Private/" "Public/")    
set(INCLUDE_BOOST "Boost/")

file(GLOB_RECURSE LIB_FILES 
    "Private/*.*"
    "Public/*.*"
)

################################################################################
### Add build target
################################################################################
add_library(${PROJECT_NAME} SHARED ${LIB_FILES} ${INCLUDE_DIRS})

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS} ${INCLUDE_BOOST})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_DLL)

install(TARGETS ${PROJECT_NAME} LIBRARY RUNTIME DESTINATION ${EMU_INSTALL_DIR}/)
