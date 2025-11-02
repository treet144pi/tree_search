cmake_minimum_required(VERSION 3.16)
project(TreeSearchAVL CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
add_library(trees INTERFACE)
target_include_directories(trees INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(func_tree src/func_tree.cpp src/runner.cpp)
target_include_directories(func_tree PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(func_tree PRIVATE trees)

add_executable(bench_tree src/bench_tree.cpp src/runner.cpp)
target_include_directories(bench_tree PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(bench_tree PRIVATE trees)

add_executable(func_set src/func_set.cpp src/runner_set.cpp)
target_include_directories(func_set PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(bench_set src/bench_set.cpp src/runner_set.cpp)
target_include_directories(bench_set PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(CTest)
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()
