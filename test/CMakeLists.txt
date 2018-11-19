CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(TestDing
        LANGUAGES CXX)


add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)

if (EXISTS ${GOOGLETEST_DIR})
    SET(GTestSrc ${GOOGLETEST_DIR}/googletest)
    SET(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (EXISTS $ENV{GOOGLETEST_DIR})
    SET(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
    SET(GTestSrc ${GOOGLETEST_DIR}/googletest)
    SET(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (EXISTS ${CMAKE_SOURCE_DIR}/../googletest)
    message(WARNING "Using googletest src dir specified at ${CMAKE_SOURCE_DIR}/../googletest")
    SET(GOOGLETEST_DIR ${CMAKE_SOURCE_DIR}/../googletest)
    SET(GTestSrc ${GOOGLETEST_DIR}/googletest)
    SET(GMockSrc ${GOOGLETEST_DIR}/googlemock)
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

message(STATUS "GOOGLETEST_DIR:${GOOGLETEST_DIR}")
include_directories(
    ${GTestSrc}
    ${GTestSrc}/include
    ${GMockSrc}
    ${GMockSrc}/include
    ../include)

file(GLOB TEST_SRCS "*.cpp")
list(SORT TEST_SRCS)

add_executable(${PROJECT_NAME} ${TEST_SRCS}
               ${GTestSrc}/src/gtest-all.cc
               ${GMockSrc}/src/gmock-all.cc)
add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads ding)

