cmake_minimum_required(VERSION 3.15)
project(client_console_app)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc)
add_executable(client_console_app main.cpp src/client_app.cpp inc/client_app.h src/file.cpp inc/file.h)