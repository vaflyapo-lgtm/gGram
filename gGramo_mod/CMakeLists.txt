cmake_minimum_required(VERSION 3.16)
project(gGramo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Svg)

find_library(TDJSON_LIB tdjson PATHS /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu)

add_executable(gGramo
    src/main.cpp
)

target_link_libraries(gGramo PRIVATE
    Qt5::Widgets
    Qt5::Svg
    ${TDJSON_LIB}
    pthread
)
