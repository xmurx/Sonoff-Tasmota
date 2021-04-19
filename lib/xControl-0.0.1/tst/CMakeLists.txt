cmake_minimum_required(VERSION 3.2.0)

# project(xArduinoSupport)
# file( TO_CMAKE_PATH "$ENV{USERPROFILE}/.platformio/packages/framework-arduinoespressif8266/cores/esp8266" ArduinoCoreDir )
# file( TO_CMAKE_PATH "$ENV{USERPROFILE}/..platformio/packages/framework-arduinoespressif8266/tools/sdk/" ArduinoSDKDir )
# message( "Full Arduino Path: ${ArduinoCoreDir}")
# message( "Full ArduinoSDKDir Path: ${ArduinoSDKDir}")
# add_library(xArduinoSupport ${ArduinoCoreDir}/WString.h
#                             ${ArduinoCoreDir}/WString.cpp )
# set_property(TARGET xArduinoSupport PROPERTY CXX_STANDARD 11)
# target_include_directories(xArduinoSupport PUBLIC ${ArduinoCoreDir}
#                                                   ${ArduinoSDKDir}/include
#                                                   ${ArduinoSDKDir}/libc/xtensa-lx106-elf/include/ )

project(xControl-Unittest)
add_executable(xControl-Unittest ./src/label.cpp
                                 ./src/time.cpp
                                 ./src/StateControl.cpp
                                 ./mocks/tasmota.cpp
                                 ./mocks/tasmota.h
                                 ./src/view_unittest.cpp
                                 )

target_include_directories(xControl-Unittest PUBLIC ${gmock_SOURCE_DIR}/include ../src/ ./mocks ./inc ./src .)
set_property(TARGET xControl-Unittest PROPERTY CXX_STANDARD 11)

target_link_libraries(xControl-Unittest xControl gtest_main gmock)
add_test(NAME xControl-Unittest COMMAND xControl_Ut)
add_custom_command(TARGET xControl-Unittest POST_BUILD COMMAND opencppcoverage --sources *xControl* --excluded_sources *googletest* --export_type=cobertura:${CMAKE_SOURCE_DIR}/cov_xml.xml -- $<TARGET_FILE:xControl-Unittest>)
