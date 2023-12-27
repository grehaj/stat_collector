cmake_minimum_required(VERSION 3.22)

project(stat_collector VERSION 1.0)

include_directories(include)

add_definitions(-std=c++17)

set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -g -Wall")

add_subdirectory(lib/collector)
add_subdirectory(lib/network)
add_subdirectory(lib/utils)

add_executable(stat_server app/server.cpp)
add_executable(stat_client app/client.cpp)

target_link_libraries(stat_client PUBLIC collector network utils)
target_link_libraries(stat_server PUBLIC network)

#tests
set(GMOCK_PATH /home/gregor85/Programming/libs/gmock-1.7.0)
include_directories(${GMOCK_PATH}/include ${GMOCK_PATH}/gtest/include)
link_directories(${GMOCK_PATH}/mybuild ${GMOCK_PATH}/gtest/mybuild)

add_executable(ut tests/main.cpp
                  tests/TimePrinterTest.cpp
                  tests/TimeUtilsTest.cpp)

target_link_libraries(ut utils)
target_link_libraries(ut pthread)
target_link_libraries(ut gmock)
target_link_libraries(ut gtest)



