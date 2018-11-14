cmake_minimum_required(VERSION 2.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
project(DingLibrary)

#set source files
file(GLOB CORE_SRCS "*.cpp" "*.c")
list(SORT CORE_SRCS)

# file(GLOB LOGGING_SRCS "logging/*.cpp")
# list(SORT LOGGING_SRCS)

# file(GLOB DRIVER_SRCS "driver/*.cpp")
# list(SORT DRIVER_SRCS)

# file(GLOB LOGIC_SRCS "logic/*.cpp")
# list(SORT LOGIC_SRCS)

# file(GLOB TIMER_SRCS "timer/*.cpp")
# list(SORT TIMER_SRCS)

# file(GLOB SM_SRCS "statemachine/*.cpp")
# list(SORT SM_SRCS)

#add target
# add_library(ding SHARED ${CORE_SRCS} ${TIMER_SRCS}
    # ${LOGGING_SRCS} ${DRIVER_SRCS} ${LOGIC_SRCS} ${SM_SRCS})
add_library(ding SHARED ${CORE_SRCS})

set_target_properties(ding
    PROPERTIES
    VERSION     1.0
    SOVERSION   1
    )

#set version generation task
IF (WIN32)
#    ADD_CUSTOM_TARGET(
#        genVersion
#        COMMAND C:/Windows/System32/cmd.exe /C ${CMAKE_SOURCE_DIR}/src/version.bat
#    )
ELSEIF (UNIX)
    set(USER_LINK_LIBS pthread rt)
#    ADD_CUSTOM_TARGET(
#        genVersion
#        COMMAND bash ${CMAKE_SOURCE_DIR}/src/ver_gen.sh
#    )
ENDIF ()
#ADD_DEPENDENCIES(ding genVersion)

#link target libraries
target_link_libraries(ding ${USER_LINK_LIBS})
