cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)

include_directories(../include)

function(generate_template_h LC_FILENAME LC_C_TYPE)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/include/clambdacommon/maths/geometry/${LC_FILENAME}.h.in"
            "${CMAKE_BINARY_DIR}/c_wrapper/exports/clambdacommon/maths/geometry/${LC_FILENAME}_${LC_C_TYPE}.h"
            IMMEDIATE @ONLY)
endfunction()

function(generate_template_cpp LC_FILENAME LC_C_TYPE)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/src/maths/geometry/${LC_FILENAME}.cpp.in"
            "${CMAKE_BINARY_DIR}/c_wrapper/exports/src/maths/geometry/${LC_FILENAME}_${LC_C_TYPE}.cpp"
            IMMEDIATE @ONLY)
endfunction()

function(generate_template_wrapper LC_FILENAME LC_C_TYPE)
    generate_template_h(${LC_FILENAME} ${LC_C_TYPE})
    generate_template_cpp(${LC_FILENAME} ${LC_C_TYPE})
    set(C_SOURCES_FILES "${C_SOURCES_FILES};${CMAKE_BINARY_DIR}/c_wrapper/exports/src/maths/geometry/${LC_FILENAME}_${LC_C_TYPE}.cpp" PARENT_SCOPE)
    if (LAMBDACOMMON_INSTALL)
        install(FILES ${CMAKE_BINARY_DIR}/c_wrapper/exports/clambdacommon/maths/geometry/${LC_FILENAME}_${LC_C_TYPE}.h DESTINATION include/clambdacommon/maths/geometry)
    endif ()
endfunction()

# All files
# This is the C headers files.
set(C_HEADERS_GRAPHICS include/clambdacommon/graphics/color.h)
set(C_HEADERS_MATHS include/clambdacommon/maths/geometry/point.h include/clambdacommon/maths/geometry/vector.h)
set(C_HEADERS_SYSTEM include/clambdacommon/system/os.h include/clambdacommon/system/terminal.h include/clambdacommon/system/system.h include/clambdacommon/system/time.h)
set(C_HEADERS_BASE include/clambdacommon/clambdacommon.h include/clambdacommon/lstring.h include/clambdacommon/maths.h include/clambdacommon/resources.h include/clambdacommon/types.h)
set(C_HEADERS_FILES ${C_HEADERS_GRAPHICS} ${C_HEADERS_MATHS} ${C_HEADERS_SYSTEM} ${C_HEADERS_BASE})
# This is the C sources files.
set(C_SOURCES_GRAPHICS src/graphics/color.cpp)
set(C_SOURCES_SYSTEM src/system/os.cpp src/system/terminal.cpp src/system/system.cpp src/time.cpp)
set(C_SOURCES_BASE src/clambdacommon.cpp src/lstring.cpp src/maths.cpp src/resources.cpp)
set(C_SOURCES_FILES ${C_SOURCES_GRAPHICS} ${C_SOURCES_SYSTEM} ${C_SOURCES_BASE})

generate_template_wrapper(point i8)
generate_template_wrapper(point u8)
generate_template_wrapper(point i16)
generate_template_wrapper(point u16)
generate_template_wrapper(point i32)
generate_template_wrapper(point u32)
generate_template_wrapper(point i64)
generate_template_wrapper(point u64)
generate_template_wrapper(point f32)
generate_template_wrapper(point f64)
generate_template_wrapper(vector i8)
generate_template_wrapper(vector u8)
generate_template_wrapper(vector i16)
generate_template_wrapper(vector u16)
generate_template_wrapper(vector i32)
generate_template_wrapper(vector u32)
generate_template_wrapper(vector i64)
generate_template_wrapper(vector u64)
generate_template_wrapper(vector f32)
generate_template_wrapper(vector f64)

# Now build the library
# Build the shared library
add_library(clambdacommon ${C_HEADERS_FILES} ${C_SOURCES_FILES})
target_link_libraries(clambdacommon lambdacommon)

target_include_directories(clambdacommon PUBLIC "include")
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