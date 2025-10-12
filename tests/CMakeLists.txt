include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests unit/tree_test.cpp)
target_link_libraries(unit_tests PRIVATE trees gtest gtest_main)
add_test(NAME unit_all COMMAND unit_tests)
