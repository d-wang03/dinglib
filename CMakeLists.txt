cmake_minimum_required(VERSION 3.10)
project(dinglib)

set(CMAKE_CXX_STANDARD 14)

message(${PROJECT_SOURCE_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_DEBUG_POSTFIX _g)

include_directories(include)

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(test)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    include(CodeCoverage)
    setup_target_for_coverage(coverage TestDing coverage)
endif()