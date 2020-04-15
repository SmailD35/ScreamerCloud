cmake_minimum_required(VERSION 3.15)
project(network)
set(CMAKE_CXX_STANDARD 14)
enable_testing()

add_subdirectory(tests)
add_executable(network src/main.cpp src/network_server.cpp src/network_client.cpp headers/network_client.h tests/src/test.cpp tests/headers/test.h)

add_test(NAME unit_test COMMAND test_network)