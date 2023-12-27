cmake_minimum_required(VERSION 3.22)

project("stat_collector")

add_definitions(-std=c++17)

add_library(network STATIC
   ${CMAKE_CURRENT_SOURCE_DIR}/UnixStreamClient.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/UnixStreamServer.cpp
)

target_include_directories(collector PUBLIC ${CMAKE_SOURCE_DIR}/include/net)


