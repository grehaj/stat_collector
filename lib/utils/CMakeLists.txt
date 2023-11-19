cmake_minimum_required(VERSION 3.22)

project("stat_collector")

add_definitions(-std=c++17)

add_library(utils STATIC
   ${CMAKE_CURRENT_SOURCE_DIR}/TimePrinter.cpp
)

target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include/utils)


