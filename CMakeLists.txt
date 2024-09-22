cmake_minimum_required(VERSION 3.28)
project(si++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(demo demo.cpp)
target_include_directories(demo PUBLIC include/)
target_sources(demo PUBLIC
    FILE_SET CXX_MODULES
    FILES
    "include/si++/si++.cppm"
    "include/si++/Rational.cppm"
)
target_link_libraries(demo fmt)
