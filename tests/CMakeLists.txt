cmake_minimum_required(VERSION 3.1)
project(λcommon_test)

set(CMAKE_CXX_STANDARD 11)

include_directories(../include)

add_executable(lambdacommon_test test.cpp)
target_link_libraries(lambdacommon_test lambdacommon)