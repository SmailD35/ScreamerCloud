cmake_minimum_required(VERSION 3.15)
project(db_app_test CXX)

set(CMAKE_CXX_STANDARD 14)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

include_directories(BEFORE ${GTEST_INCLUDE_DIRS})
include_directories(../inc)
link_directories(../cmake-build-debug)

add_executable(db_app_test test.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(db_app_test gtest_main gmock_main pq ${Boost_LIBRARIES} libdb_app.dylib)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(db_app_test gtest_main gmock_main libdb_app.so)
endif()
#add_executable(db_app_test ../src/database_wrapper.cpp ../src/users_database_manager.cpp ../src/files_database_manager.cpp ../src/database_manager.cpp ../inc/database_manager.h test.cpp )
#target_link_libraries(db_app_test gtest_main gmock_main pq ${Boost_LIBRARIES})
