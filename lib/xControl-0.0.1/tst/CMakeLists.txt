cmake_minimum_required(VERSION 2.8.2)

project(xControl)
add_library(xControl sample1.cc sample1.h)

project(xControl-Unittest)
add_executable(xControl-Unittest sample1_unittest.cc)
target_link_libraries(xControl-Unittest xControl gtest_main)
add_test(NAME xControl-Unittest COMMAND xControl_Ut)