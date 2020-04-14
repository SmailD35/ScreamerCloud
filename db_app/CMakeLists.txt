cmake_minimum_required(VERSION 3.15)
project(db_app CXX)

set(CMAKE_CXX_STANDARD 14)
enable_testing()
add_subdirectory(test)

#include_directories(inc)

add_executable(db_app main.cpp src/database_manager.cpp inc/database_manager.h src/users_database_manager.cpp inc/users_database_manager.h src/files_database_manager.cpp inc/files_database_manager.h inc/interfaces.h)
add_test(NAME db_app_test COMMAND test)