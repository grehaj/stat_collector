cmake_minimum_required(VERSION 3.22)

project("stat_collector")

add_definitions(-std=c++17)

add_library(network STATIC
   ${CMAKE_CURRENT_SOURCE_DIR}/ClientSocket.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/ServerSocket.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/SocketUtils.cpp
)

target_include_directories(network PUBLIC ${CMAKE_SOURCE_DIR}/include/network)
