set(CMAKE_C_STANDARD 11)

include_directories(../include)

if (WIN32)
	set(LCOMMON_ICON lambdacommon.rc)
endif ()

add_executable(clambdacommon_test test.c ${LCOMMON_ICON})
target_link_libraries(clambdacommon_test clambdacommon lambdacommon)