cmake_minimum_required(VERSION 3.11-3.18)

project(src)

add_library(solution generic/solution.cpp)
target_include_directories(solution PUBLIC ${PROJECT_SOURCE_DIR})

add_library(instance generic/instance.cpp)
target_include_directories(instance PUBLIC ${PROJECT_SOURCE_DIR})

add_library(optimizer generic/optimizer.cpp)
target_include_directories(optimizer PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(optimizer PRIVATE solution instance)

add_library(tsp specific/tsp.cpp)
target_include_directories(tsp PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(tsp PRIVATE instance)
