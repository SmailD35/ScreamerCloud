cmake_minimum_required(VERSION 3.15)
project(server)
enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -I/usr/include/postgresql)

find_package(Boost REQUIRED COMPONENTS filesystem log log_setup)

include_directories(include ../db_app/inc ../utills ../network/network_server/headers ${Boost_INCLUDE_DIRS})
link_directories(../utills/cmake-build-debug ../network/cmake-build-debug ../db_app/cmake-build-debug)

add_subdirectory(tests)
add_executable(server source/main.cpp source/commands.cpp include/commands.h source/server.cpp include/server.h include/server_logging.hpp)
target_link_libraries(server screamer_utills network_server db_app gmock_main ${Boost_LIBRARIES})

add_test(NAME unit_test COMMAND test_server)