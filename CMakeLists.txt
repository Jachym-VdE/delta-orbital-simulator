cmake_minimum_required(VERSION 3.28)
project(AeroSim-Plus-Plus)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics REQUIRED)

add_executable(main src/main.cpp)
target_link_libraries(main SFML::Graphics)