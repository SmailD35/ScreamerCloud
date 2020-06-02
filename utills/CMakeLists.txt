cmake_minimum_required(VERSION 3.15)
project(screamer_utills)

set(CMAKE_CXX_STANDARD 14)

add_library(screamer_utills SHARED file.cpp file.h cmd_codes.h error_codes.h)

add_executable(test_utills test.cpp)
target_link_libraries(test_utills screamer_utills)