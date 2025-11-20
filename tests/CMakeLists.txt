find_package(GTest REQUIRED)

add_executable(unit_tests unit/tree_test.cpp)
target_link_libraries(unit_tests PRIVATE trees GTest::gtest GTest::gtest_main)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME unit_all COMMAND unit_tests)

add_executable(e2e
  ${CMAKE_CURRENT_SOURCE_DIR}/e2e_runner.cpp
  ${CMAKE_SOURCE_DIR}/src/runner.cpp
)
target_include_directories(e2e PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(e2e PRIVATE trees)

target_compile_definitions(e2e PRIVATE TESTS_DIR="${CMAKE_SOURCE_DIR}/tests")
