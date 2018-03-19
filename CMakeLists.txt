cmake_minimum_required(VERSION 2.8.12)

project(curves LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(COVERAGE OFF CACHE BOOL "Coverage")

add_subdirectory("external/googletest")
add_subdirectory("test")

set(COMMON_LIBS )

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
if (COVERAGE)
    set(COMMON_LIBS gcov ${COMMON_LIBS})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(SRC
    "src/chaikins.cpp"
    "src/chaikins_model.cpp"
)

set(INC
    "include/chaikins.h"
    )

include_directories(include)

add_library(line  "src/line.cpp" "include/line.h")
target_link_libraries(line ${COMMON_LIBS} Qt5::Core  Qt5::Quick)

add_library(chaikins ${SRC} ${INC})
target_link_libraries(chaikins Qt5::Quick)

add_executable(${PROJECT_NAME} "src/main.cpp" "qml.qrc")

target_link_libraries(${PROJECT_NAME} line chaikins Qt5::Core Qt5::Quick)
