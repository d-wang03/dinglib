cmake_minimum_required(VERSION 3.10)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
project(DingLibrary VERSION 0.1.1)
# add_compile_options(-O0 -fprofile-arcs -ftest-coverage)
# add_link_options(-fprofile-arcs)
#set source files
file(GLOB CORE_SRCS "*.cpp" "*.c")
list(SORT CORE_SRCS)

add_library(ding ${CORE_SRCS})

# set_target_properties(ding
#     PROPERTIES
#     VERSION     1.0
#     SOVERSION   1
#     )

#set version generation task
IF (WIN32)
ELSEIF (UNIX)
    set(USER_LINK_LIBS pthread rt)
ENDIF ()

#link target libraries
target_link_libraries(ding ${USER_LINK_LIBS})
