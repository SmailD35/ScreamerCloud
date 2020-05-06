cmake_minimum_required(VERSION 3.15)
project(client_console_app)

set(CMAKE_CXX_STANDARD 14)
enable_testing()
add_subdirectory(tests)

find_package( Boost REQUIRED COMPONENTS program_options )

include_directories(inc tests/inc ${Boost_INCLUDE_DIRS})
add_executable(client_console_app main.cpp src/client_app.cpp inc/client_app.h inc/file.h src/file.cpp)
target_link_libraries(client_console_app gmock_main ${Boost_LIBRARIES})
add_test(NAME client_console_app_test COMMAND test)