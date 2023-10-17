cmake_minimum_required(VERSION 3.22)

project(stat_collector VERSION 1.0)

include_directories(include)

add_definitions(-std=c++17)

set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -g -Wall")

add_subdirectory(lib/collector)

add_executable(stat_server app/server.cpp)
add_executable(stat_client app/client.cpp)
target_link_libraries(stat_client PUBLIC collector)


