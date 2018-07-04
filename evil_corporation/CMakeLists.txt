cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(evil.corp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/configuration/cmake/")

include(platform_dependencies)
include(sources_parsing)

platform_setup()

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
include_directories(${PROJECT_SOURCE_DIR})

conan_basic_setup()

add_catalog_recursive(${PROJECT_SOURCE_DIR}/src /src SOURCES)
add_catalog_recursive(${PROJECT_SOURCE_DIR}/external_api /external_api SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})

foreach(debug_lib ${CONAN_LIBS_DEBUG})
    target_link_libraries(${PROJECT_NAME} debug ${debug_lib})
endforeach()
foreach(release_lib ${CONAN_LIBS_RELEASE})
    target_link_libraries(${PROJECT_NAME} optimized ${release_lib})
endforeach()