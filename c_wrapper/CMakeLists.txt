cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)

include_directories(../include)

# All files
# This is the C headers files.
set(C_HEADERS_GRAPHICS include/clambdacommon/graphics/color.h)
set(C_HEADERS_MATHS include/clambdacommon/maths/geometry/point.h)
set(C_HEADERS_SYSTEM include/clambdacommon/system/os.h include/clambdacommon/system/terminal.h include/clambdacommon/system/system.h include/clambdacommon/system/time.h)
set(C_HEADERS_BASE include/clambdacommon/clambdacommon.h include/clambdacommon/lstring.h include/clambdacommon/maths.h include/clambdacommon/resources.h)
set(C_HEADERS_FILES ${C_HEADERS_GRAPHICS} ${C_HEADERS_MATHS} ${C_HEADERS_SYSTEM} ${C_HEADERS_BASE})
# This is the C sources files.
set(C_SOURCES_GRAPHICS src/graphics/color.cpp)
set(C_SOURCES_SYSTEM src/system/os.cpp src/system/terminal.cpp src/system/system.cpp src/time.cpp)
set(C_SOURCES_BASE src/clambdacommon.cpp src/lstring.cpp src/maths.cpp src/resources.cpp)
set(C_SOURCES_FILES ${C_SOURCES_GRAPHICS} ${C_SOURCES_SYSTEM} ${C_SOURCES_BASE})

function(generate_point_h LC_C_POINT_TYPE)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/include/clambdacommon/maths/geometry/point.h.in"
            "${CMAKE_BINARY_DIR}/c_wrapper/exports/clambdacommon/maths/geometry/point_${LC_C_POINT_TYPE}.h"
            IMMEDIATE @ONLY)
endfunction()

function(generate_point_cpp LC_C_POINT_TYPE)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/src/maths/geometry/point.cpp.in"
            "${CMAKE_BINARY_DIR}/c_wrapper/exports/src/maths/geometry/point_${LC_C_POINT_TYPE}.cpp"
            IMMEDIATE @ONLY)
endfunction()

function(generate_point_wrapper LC_C_POINT_TYPE)
    generate_point_h(${LC_C_POINT_TYPE})
    generate_point_cpp(${LC_C_POINT_TYPE})
    set(C_SOURCES_FILES "${C_SOURCES_FILES};${CMAKE_BINARY_DIR}/c_wrapper/exports/src/maths/geometry/point_${LC_C_POINT_TYPE}.cpp" PARENT_SCOPE)
    if (LAMBDACOMMON_INSTALL)
        install(FILES ${CMAKE_BINARY_DIR}/c_wrapper/exports/clambdacommon/maths/geometry/point_${LC_C_POINT_TYPE}.h DESTINATION include/clambdacommon/maths/geometry)
    endif ()
endfunction()

generate_point_wrapper(int8_t)
generate_point_wrapper(uint8_t)
generate_point_wrapper(int16_t)
generate_point_wrapper(uint16_t)
generate_point_wrapper(int32_t)
generate_point_wrapper(uint32_t)
generate_point_wrapper(int64_t)
generate_point_wrapper(uint64_t)
generate_point_wrapper(float)
generate_point_wrapper(double)

# Now build the library
# Build the shared library
add_library(clambdacommon ${C_HEADERS_FILES} ${C_SOURCES_FILES})
target_link_libraries(clambdacommon lambdacommon)

target_include_directories(clambdacommon PUBLIC "${CMAKE_BINARY_DIR}/c_wrapper/exports")

# Install if the option is on.
if (LAMBDACOMMON_INSTALL)
    foreach (LOOP_HEADER ${C_HEADERS_FILES})
        get_filename_component(HEADER_PARENT_DIR ${LOOP_HEADER} DIRECTORY)
        install(FILES ${LOOP_HEADER} DESTINATION ${HEADER_PARENT_DIR})
    endforeach ()
    install(TARGETS clambdacommon
            RUNTIME DESTINATION bin COMPONENT libraries
            LIBRARY DESTINATION lib COMPONENT libraries
            ARCHIVE DESTINATION lib COMPONENT libraries)
endif ()

if (LAMBDACOMMON_BUILD_TESTS)
    add_subdirectory(tests)
endif ()