cmake_minimum_required(VERSION 3.15)
project(server)
enable_testing()
set(CMAKE_CXX_STANDARD 14)

include_directories(include tests/include)

add_subdirectory(tests)
add_executable(server source/main.cpp source/commands.cpp include/commands.h source/server.cpp include/server.h)
target_link_libraries(server gmock_main)

add_test(NAME unit_test COMMAND test_server)
