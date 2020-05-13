cmake_minimum_required(VERSION 3.15)
project(network)
set(CMAKE_CXX_STANDARD 14)
enable_testing()

add_subdirectory(tests)

add_library(network_client SHARED network_client/src/network_client.cpp network_client/headers/network_client.h)
add_library(network_server SHARED network_server/src/network_server.cpp network_server/headers/network_server.h)

add_test(NAME unit_test COMMAND test_network)