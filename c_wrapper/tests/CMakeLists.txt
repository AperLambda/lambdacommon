set(CMAKE_C_STANDARD 11)

include_directories(../include)

add_executable(clambdacommon_test test.c)
target_link_libraries(clambdacommon_test clambdacommon lambdacommon)