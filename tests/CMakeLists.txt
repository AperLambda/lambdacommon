cmake_minimum_required(VERSION 3.1)
project(λcommon_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(../include)

if (WIN32)
	set(LCOMMON_ICON lambdacommon.rc)
endif ()

add_executable(lambdacommon_test test.cpp ${LCOMMON_ICON})
target_link_libraries(lambdacommon_test lambdacommon)