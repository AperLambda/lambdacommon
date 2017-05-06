cmake_minimum_required(VERSION 3.1)
project(apercommon)

include(GenerateExportHeader)

set(CMAKE_CXX_STANDARD 14)

option(APERCOMMON_BUILD_STATIC "Build static libraries" OFF)
option(APERCOMMON_INSTALL "Generate installation target" ON)
option(APERCOMMON_BUILD_TESTS "Build the AperCommon test programs" ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -mtune=native -ffast-math -D__extern_always_inline=\"extern __always_inline\"")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -mtune=native -ffast-math")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()

# All files
# There is the C++ headers files.
set(HEADERS_GRAPHICS include/apercommon/graphics/color.h)
set(HEADERS_SYSTEM include/apercommon/system/os.h include/apercommon/system/devices.h include/apercommon/system/terminal.h)
set(HEADERS_BASE include/apercommon/apercommon.h include/apercommon/string.h)
set(HEADERS_FILES ${HEADERS_GRAPHICS} ${HEADERS_SYSTEM} ${HEADERS_BASE})
# There is the C++ sources files.
set(SOURCES_GRAPHICS src/graphics/color.cpp)
set(SOURCES_SYSTEM src/system/os.cpp src/system/terminal.cpp)
set(SOURCES_BASE src/apercommon.cpp src/string.cpp)
set(SOURCES_FILES ${SOURCES_GRAPHICS} ${SOURCES_SYSTEM} ${SOURCES_BASE})

# Now build the library
# Build static if the option is on.
if (APERCOMMON_BUILD_STATIC)
    add_library(apercommon_static STATIC ${HEADERS_FILES} ${SOURCES_FILES})
endif ()
# Build the shared library
add_library(apercommon SHARED ${HEADERS_FILES} ${SOURCES_FILES})
# Generate the export header and include it.
GENERATE_EXPORT_HEADER(apercommon
        BASE_NAME mapercommon
        EXPORT_MACRO_NAME APERCOMMON_API
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/apercommon_exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
target_include_directories(apercommon PUBLIC ${CMAKE_BINARY_DIR}/exports)
if (APERCOMMON_BUILD_STATIC)
    target_compile_definitions(apercommon_static PUBLIC -DSHARED_EXPORTS_BUILT_AS_STATIC)
    target_include_directories(apercommon_static PUBLIC ${CMAKE_BINARY_DIR}/exports)
endif ()

# Set the variables
set(APERCOMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Install if the option is on.
if (APERCOMMON_INSTALL)
    install(FILES ${HEADERS_FILES} DESTINATION include)
    install(TARGETS apercommon
            RUNTIME DESTINATION bin COMPONENT libraries
            LIBRARY DESTINATION lib COMPONENT libraries
            ARCHIVE DESTINATION lib COMPONENT libraries)
endif ()

# Build the tests if the option is on.
if (APERCOMMON_BUILD_TESTS)
    add_subdirectory(tests)
endif ()