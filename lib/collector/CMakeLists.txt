cmake_minimum_required(VERSION 3.22)

project("stat_collector")

add_definitions(-std=c++17)


find_package(nlohmann_json 3.2.0 REQUIRED)

add_library(collector STATIC
   ${CMAKE_CURRENT_SOURCE_DIR}/Collector.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/CollectorThread.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/JsonConversion.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/TrafficData.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/TrafficReader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/TrafficStorage.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/TrafficWritter.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
)

target_include_directories(collector PUBLIC ${CMAKE_SOURCE_DIR}/include/collector)
target_link_libraries(collector PRIVATE nlohmann_json::nlohmann_json)


