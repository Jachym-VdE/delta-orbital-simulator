cmake_minimum_required(VERSION 3.28)
project(Delta-Orbital-Simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
)

FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE SFML::Graphics)