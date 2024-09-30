cmake_minimum_required(VERSION 3.11-3.18)

project(apps)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE optimizer tsp)