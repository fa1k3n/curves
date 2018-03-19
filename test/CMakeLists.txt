find_package(Threads REQUIRED)

find_package(Qt5 COMPONENTS Core Quick Test REQUIRED)

if ($ENV{GOOGLETEST_DIR})
    SET(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    SET(GOOGLETEST_DIR "${PROJECT_SOURCE_DIR}/external/googletest")
    message(STATUS "using google test at ${GOOGLETEST_DIR}")
endif ()
if (EXISTS ${GOOGLETEST_DIR})
    SET(GTestSrc ${GOOGLETEST_DIR}/googletest)
    SET(GMockSrc ${GOOGLETEST_DIR}/googlemock)
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
include_directories(${GTestSrc} ${GTestSrc}/include ${GMockSrc} ${GMockSrc}/include)

set(TESTCASES
    tst_line.cpp
    tst_chaikinsmodel.cpp
    tst_chaikinscurve.cpp
    )
add_executable(${PROJECT_NAME}_test main.cpp ${TESTCASES}
               ${GTestSrc}/src/gtest-all.cc
               ${GMockSrc}/src/gmock-all.cc)
add_test(${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test  PRIVATE line chaikins Qt5::Core Qt5::Quick Qt5::Test Threads::Threads)

