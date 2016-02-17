cmake_minimum_required(VERSION 3.4)
project(cpptest)
add_executable(cpptest test.cpp)
set(CMAKE_CXX_FLAGS "--std=c++11")
set(CMAKE_CXX_STANDARD 11)

add_executable(cpptest2 test2.cpp)
