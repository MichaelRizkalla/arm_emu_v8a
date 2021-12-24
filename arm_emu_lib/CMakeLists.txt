################################################################################
### Project name
################################################################################
set(CMAKE_CXX_STANDARD 23)
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
### Visual studio filters
################################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    #source_group("components" FILES ${EMU_COMPONTENTS})
endif()

################################################################################
### Configure target
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS} ${INCLUDE_BOOST})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DARMEMU_BUILD_DLL)

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

install(TARGETS ${PROJECT_NAME} LIBRARY RUNTIME DESTINATION ${EMU_INSTALL_DIR}/)
