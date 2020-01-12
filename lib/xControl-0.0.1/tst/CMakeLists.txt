cmake_minimum_required(VERSION 2.8.2)

project(xControl-Unittest)
add_executable(xControl-Unittest label_unittest.cpp)
target_include_directories(xControl-Unittest PUBLIC ${gmock_SOURCE_DIR}/include ../src/)
set_property(TARGET xControl-Unittest PROPERTY CXX_STANDARD 11)

target_link_libraries(xControl-Unittest xControl gtest_main gmock)
add_test(NAME xControl-Unittest COMMAND xControl_Ut)
add_custom_command(TARGET xControl-Unittest POST_BUILD COMMAND opencppcoverage --export_type cobertura $<TARGET_FILE:xControl-Unittest>)