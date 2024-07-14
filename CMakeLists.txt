cmake_minimum_required(VERSION 3.22)

project(stat_collector VERSION 1.0)

include_directories(include)

add_definitions(-std=c++17)

find_package(nlohmann_json 3.2.0 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -g -Wall")

add_subdirectory(lib/utils)

add_executable(stat_server app/server_main.cpp
                           src/server/ServerRunGuard.cpp
                           src/server/StatServer.cpp)
add_executable(stat_client app/client_main.cpp
                           src/client/StatClient.cpp
                           src/client/TrafficStorage.cpp
                           src/client/capture/AllDevs.cpp
                           src/client/capture/Capture.cpp)

target_include_directories(stat_client PUBLIC include/client)
target_include_directories(stat_client PUBLIC include/client/capture)
target_include_directories(stat_server PUBLIC include/server)

target_link_libraries(stat_client PRIVATE nlohmann_json::nlohmann_json pcap)

target_link_libraries(stat_client PUBLIC utils)
target_link_libraries(stat_server PUBLIC utils)

#tests
set(GMOCK_PATH /home/gregor85/Programming/libs/gmock-1.7.0)
include_directories(${GMOCK_PATH}/include ${GMOCK_PATH}/gtest/include)
link_directories(${GMOCK_PATH}/mybuild ${GMOCK_PATH}/gtest/mybuild)

add_executable(ut tests/main.cpp
                  src/server/ServerRunGuard.cpp
                  src/server/StatServer.cpp
                  src/client/StatClient.cpp
                  src/client/TrafficStorage.cpp
                  src/client/capture/AllDevs.cpp
                  src/client/capture/Capture.cpp
                  tests/StatClientTest.cpp
                  tests/StatServerTest.cpp
                  tests/TimePrinterTest.cpp
                  tests/TimeUtilsTest.cpp
                  tests/TrafficStorageTest.cpp
                  tests/UtilsTest.cpp)

target_include_directories(ut PUBLIC include/client)
target_include_directories(ut PUBLIC include/client/capture)
target_include_directories(ut PUBLIC include/server)

target_link_libraries(ut utils)
target_link_libraries(ut pthread)
target_link_libraries(ut gmock)
target_link_libraries(ut gtest)
target_link_libraries(ut pcap)



