cmake_minimum_required(VERSION 3.15)
project(network)
set(CMAKE_CXX_STANDARD 14)
#enable_testing()

#add_subdirectory(tests)

find_package(Boost REQUIRED COMPONENTS system serialization)
include_directories(network_client/headers network_server/headers ../utills ${Boost_INCLUDE_DIRS})
link_directories(../utills/cmake-build-debug/)

add_library(network_client SHARED network_client/src/network_client.cpp network_client/headers/network_client.h)
target_link_libraries(network_client screamer_utills ${Boost_LIBRARIES})

add_library(network_server SHARED network_server/src/network_server.cpp network_server/headers/network_server.h)
target_link_libraries(network_server screamer_utills ${Boost_LIBRARIES})

add_executable(test_server tests/test_server.cpp)
target_link_libraries(test_server network_server screamer_utills)

add_executable(test_client tests/test_client.cpp)
target_link_libraries(test_client network_client screamer_utills)

#add_test(NAME unit_test COMMAND test_network)