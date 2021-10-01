cmake_minimum_required(VERSION 3.10.0)

project(kmeans_c C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_BUILD_TYPE Debug)

# binary folder after building
set(BIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BIN_OUTPUT_DIR})

# source code at root
set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.c
    ${PROJECT_SOURCE_DIR}/pcg_basic.h
    ${PROJECT_SOURCE_DIR}/pcg_basic.c
    ${PROJECT_SOURCE_DIR}/kmeans.h
    ${PROJECT_SOURCE_DIR}/kmeans.c
    ${PROJECT_SOURCE_DIR}/ui.h
    ${PROJECT_SOURCE_DIR}/ui.c
    ${PROJECT_SOURCE_DIR}/kmeans_opencl.h
    ${PROJECT_SOURCE_DIR}/kmeans_opencl.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

configure_file(${PROJECT_SOURCE_DIR}/plot.py ${BIN_OUTPUT_DIR}/plot.py COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/pcg.cl ${BIN_OUTPUT_DIR}/pcg.cl COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/kmeans.cl ${BIN_OUTPUT_DIR}/kmeans.cl COPYONLY)

add_custom_command(
    TARGET ${PROJECT_NAME}  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/plot.py
        ${BIN_OUTPUT_DIR}/plot.py)
add_custom_command(
    TARGET ${PROJECT_NAME}  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/pcg.cl
        ${BIN_OUTPUT_DIR}/pcg.cl)
add_custom_command(
    TARGET ${PROJECT_NAME}  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/kmeans.cl
        ${BIN_OUTPUT_DIR}/kmeans.cl)
    

add_subdirectory(data_structures)

# includes
target_include_directories(${PROJECT_NAME} PRIVATE ./)

# Linux specifics
if (UNIX)
    # Math lib
    target_link_libraries(${PROJECT_NAME} m)
endif (UNIX)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})
target_link_libraries (${PROJECT_NAME} ${OpenCL_LIBRARY})

# Windows specifics
if (WIN32)
    include_directories(${PROJECT_NAME} 
    ${CMAKE_CURRENT_LIST_DIR}/third_party/include)
    target_link_libraries(${PROJECT_NAME} 
    ${CMAKE_CURRENT_LIST_DIR}/third_party/lib/x64/SDL2.lib
    ${CMAKE_CURRENT_LIST_DIR}/third_party/lib/x64/SDL2main.lib)

    get_target_property(sources ${PROJECT_NAME} SOURCES)
    foreach(file ${sources})
        get_filename_component(path "${file}" ABSOLUTE)
        get_filename_component(path "${path}" PATH)
        if(file MATCHES "${PROJECT_BINARY_DIR}")
            file(RELATIVE_PATH path ${PROJECT_BINARY_DIR} "${path}")
        else()
            file(RELATIVE_PATH path ${PROJECT_SOURCE_DIR} "${path}")
        endif()
        string(REGEX REPLACE "/" "\\\\" win_path "${path}")
        source_group("${win_path}" REGULAR_EXPRESSION "${path}/[^/\\]+\\..*")
    endforeach()
endif (WIN32)

# disable unsafe functions warnings
# add_compile_options(/w44101)
if (WIN32)
    set(CMAKE_C_FLAGS "/D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "/D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE ${CMAKE_C_FLAGS}")
endif (WIN32)

# Keep track of memory leaks
add_definitions(-DAPP_NAME="${PROJECT_NAME}")

# disable ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION true)