cmake_minimum_required(VERSION 3.10)

project(DingApp)
file(GLOB APP_SRCS "*.cpp")
list(SORT APP_SRCS)

add_executable(dingApp ${APP_SRCS})
target_link_libraries(dingApp ding)
