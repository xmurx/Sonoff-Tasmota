cmake_minimum_required(VERSION 2.8.2)

project(xControl)
add_library(xControl Label.h
                     label.cpp
                     Images.h
                     Images.cpp )

target_include_directories(xControl PUBLIC ../tst/)