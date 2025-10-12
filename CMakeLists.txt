cmake_minimum_required(VERSION 3.16)

project(TreeAVL CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(trees INTERFACE)
target_include_directories(trees INTERFACE ${CMAKE_SOURCE_DIR}/include)
add_executable(task src/main.cpp)
target_link_libraries(task PRIVATE trees)

enable_testing()
add_subdirectory(tests)
