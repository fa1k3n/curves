cmake_minimum_required(VERSION 2.8.12)

project(testqt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(SRC
    "src/main.cpp"
    "src/line.cpp"
    "src/chaikins.cpp"
    "src/chaikins_model.cpp"
)

set(INC
    "include/line.h"
    "include/chaikins.h"
    )


include_directories(include)

add_executable(${PROJECT_NAME} ${SRC} ${INC} "qml.qrc")

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
