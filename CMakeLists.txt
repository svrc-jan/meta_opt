cmake_minimum_required(VERSION 3.11-3.18)

project(meta_opt)


set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)


add_library(solution src/generic/solution.cpp)
target_include_directories(solution PUBLIC ${PROJECT_SOURCE_DIR})

add_library(instance src/generic/instance.cpp)
target_include_directories(instance PUBLIC ${PROJECT_SOURCE_DIR})

add_library(optimizer src/generic/optimizer.cpp)
target_include_directories(optimizer PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(optimizer PRIVATE solution instance)

add_library(tsp src/specific/tsp.cpp)
target_include_directories(tsp PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(tsp PRIVATE instance)

add_executable(test apps/test.cpp)
target_link_libraries(test PRIVATE optimizer tsp)

