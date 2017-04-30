cmake_minimum_required(VERSION 3.1)
project(apercommon_test)

set(CMAKE_CXX_STANDARD 11)

include_directories(../include)

add_executable(apercommon_test test.cpp)
target_link_libraries(apercommon_test apercommon)
