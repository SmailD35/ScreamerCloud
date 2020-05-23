cmake_minimum_required(VERSION 3.16)
project(screamer_utills)

set(CMAKE_CXX_STANDARD 14)

add_library(screamer_utills SHARED file.cpp file.h cmd_codes.h)