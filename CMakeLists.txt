cmake_minimum_required(VERSION 3.1)
project(apercommon)

include(GenerateExportHeader)

set(CMAKE_CXX_STANDARD 11)

option(APERCOMMON_BUILD_STATIC "Build static libraries" OFF)
option(APERCOMMON_BUILD_TESTS "Build the AperCommon test programs" ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -mtune=native -ffast-math -D__extern_always_inline=\"extern __always_inline\"")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -mtune=native -ffast-math")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()

# All files
set(HEADERS_GRAPHICS include/apercommon/graphics/color.h)
set(HEADERS_SYSTEM include/apercommon/system/os.h include/apercommon/system/devices.h include/apercommon/system/terminal.h)
set(HEADERS_BASE include/apercommon/apercommon.h include/apercommon/string.h)
set(HEADERS_FILES ${HEADERS_GRAPHICS} ${HEADERS_SYSTEM} ${HEADERS_BASE})
set(SOURCES_GRAPHICS src/graphics/color.cpp)
set(SOURCES_SYSTEM src/system/os.cpp src/system/terminal.cpp)
set(SOURCES_BASE src/apercommon.cpp src/string.cpp)
set(SOURCES_FILES ${SOURCES_GRAPHICS} ${SOURCES_SYSTEM} ${SOURCES_BASE})

if (APERCOMMON_BUILD_STATIC)
    add_library(apercommon STATIC ${HEADERS_FILES} ${SOURCES_FILES})
else ()
    add_library(apercommon SHARED ${HEADERS_FILES} ${SOURCES_FILES})
endif ()
GENERATE_EXPORT_HEADER(apercommon
        BASE_NAME mapercommon
        EXPORT_MACRO_NAME APERCOMMON_API
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/apercommon_exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
target_include_directories(apercommon PUBLIC ${CMAKE_BINARY_DIR}/exports)

if (APERCOMMON_BUILD_TESTS)
    add_subdirectory(tests)
endif ()