cmake_minimum_required(VERSION 3.15)
project(server)
enable_testing()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -I/usr/include/postgresql)

find_package(Boost REQUIRED COMPONENTS serialization system filesystem)

include_directories(include ../db_app/inc ../utills ../network/network_server/headers)
link_directories(../utills/cmake-build-debug ../network/cmake-build-debug ../db_app/cmake-build-debug)

add_subdirectory(tests)
add_executable(server source/main.cpp source/commands.cpp include/commands.h source/server.cpp include/server.h)
target_link_libraries(server screamer_utills network_server db_app gmock_main)

add_test(NAME unit_test COMMAND test_server)