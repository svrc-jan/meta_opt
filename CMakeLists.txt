cmake_minimum_required(VERSION 3.11-3.18)

project(meta_opt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GCC_COVERAGE_COMPILE_FLAGS "-W -fopenmp -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

include_directories(${PROJECT_SOURCE_DIR}/apps  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/lib)

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


add_executable(json_test apps/json_test.cpp)

