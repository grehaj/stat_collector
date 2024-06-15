cmake_minimum_required(VERSION 3.22)

project("stat_collector")

add_definitions(-std=c++17)

add_library(server STATIC
   ${CMAKE_CURRENT_SOURCE_DIR}/ServerRunGuard.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/StatServer.cpp
)

target_link_libraries(server PUBLIC utils)
