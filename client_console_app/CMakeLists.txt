cmake_minimum_required(VERSION 3.15)
project(client_console_app)

set(CMAKE_CXX_STANDARD 14)
enable_testing()
add_subdirectory(tests)

find_package( Boost REQUIRED COMPONENTS program_options filesystem )
include_directories(inc ../network/network_client/headers ../utills ${Boost_INCLUDE_DIRS})
link_directories(../network/cmake-build-debug ../utills/cmake-build-debug)

add_executable(client_console_app main.cpp src/client_app.cpp inc/client_app.h)
target_link_libraries(client_console_app network_client screamer_utills ${Boost_LIBRARIES} gmock_main)

add_test(NAME client_console_app_test COMMAND test)