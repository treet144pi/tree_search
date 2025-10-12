# CMake generated Testfile for 
# Source directory: /home/timur/с++/c++mipt/tree_search/tests
# Build directory: /home/timur/с++/c++mipt/tree_search/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(unit_all "/home/timur/с++/c++mipt/tree_search/build/tests/unit_tests")
set_tests_properties(unit_all PROPERTIES  _BACKTRACE_TRIPLES "/home/timur/с++/c++mipt/tree_search/tests/CMakeLists.txt;12;add_test;/home/timur/с++/c++mipt/tree_search/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
