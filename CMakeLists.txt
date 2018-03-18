cmake_minimum_required(VERSION 2.8.12)

project(curves LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory("external/googletest")
add_subdirectory("test")

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
target_link_libraries(line  Qt5::Core  Qt5::Quick)

add_library(chaikins ${SRC} ${INC})
target_link_libraries(chaikins Qt5::Quick)

add_executable(${PROJECT_NAME} "src/main.cpp" "qml.qrc")

target_link_libraries(${PROJECT_NAME} line chaikins Qt5::Core Qt5::Quick)
