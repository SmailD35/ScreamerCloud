cmake_minimum_required(VERSION 3.15)
project(db_app CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -I/usr/include/postgresql)

<<<<<<< HEAD
#SET(Boost_USE_STATIC_LIBS ON)
find_package( Boost REQUIRED COMPONENTS filesystem log log_setup)
include_directories(inc ${Boost_INCLUDE_DIRS})
link_directories(/usr/lib/x86_64-linux-gnu/)
add_definitions(-DBOOST_LOG_DYN_LINK)
=======
find_package( Boost REQUIRED COMPONENTS filesystem )
include_directories(inc ${Boost_INCLUDE_DIRS})
#link_directories(/usr/lib/x86_64-linux-gnu/boost)
>>>>>>> 7c4beca6dcc969c4788fad66a3a871efe0b85882

enable_testing()
add_subdirectory(test)

add_library(db_app SHARED inc/logging.hpp src/database_wrapper.cpp inc/database_wrapper.h src/users_database_manager.cpp inc/users_database_manager.h src/files_database_manager.cpp inc/files_database_manager.h src/database_manager.cpp inc/database_manager.h inc/interfaces.h)
#add_executable(db_app main.cpp src/database_wrapper.cpp inc/database_wrapper.h src/users_database_manager.cpp inc/users_database_manager.h src/files_database_manager.cpp inc/files_database_manager.h src/database_manager.cpp inc/database_manager.h inc/interfaces.h )
target_link_libraries(db_app pq ${Boost_LIBRARIES})


add_test(NAME db_app_test COMMAND test)