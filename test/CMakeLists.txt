CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(TestDing
        LANGUAGES CXX)

find_package(Threads REQUIRED)

include_directories(
    ../include)

file(GLOB TEST_SRCS "*.cpp")
list(SORT TEST_SRCS)

add_executable(${PROJECT_NAME} ${TEST_SRCS})
# add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ding gtest)

