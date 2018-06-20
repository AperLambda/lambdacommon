cmake_minimum_required(VERSION 3.8)
project(λcommon)

option(LAMBDACOMMON_BUILD_STATIC "Build static libraries" OFF)
option(LAMBDACOMMON_INSTALL "Generate installation target" ON)
option(LAMBDACOMMON_BUILD_TESTS "Build the λcommon test programs" ON)

include(GenerateExportHeader)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -mtune=native -ffast-math -D__extern_always_inline=\"extern __always_inline\"")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native -mtune=native -ffast-math")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif ()

# All files
# There is the C++ headers files.
set(HEADERS_CONNECTION include/lambdacommon/connection/address.h)
set(HEADERS_DOCUMENT include/lambdacommon/documents/document.h)
set(HEADERS_GRAPHICS include/lambdacommon/graphics/color.h)
set(HEADERS_FILESYSTEM include/lambdacommon/system/filesystem/filesystem.h)
set(HEADERS_EXCEPTIONS include/lambdacommon/exceptions/exceptions.h)
set(HEADERS_SYSTEM ${HEADERS_FILESYSTEM} include/lambdacommon/system/os.h include/lambdacommon/system/devices.h include/lambdacommon/system/terminal.h include/lambdacommon/system/system.h include/lambdacommon/system/uri.h)
set(HEADERS_BASE include/lambdacommon/lambdacommon.h include/lambdacommon/serializable.h include/lambdacommon/lstring.h include/lambdacommon/path.h include/lambdacommon/utils.h include/lambdacommon/resources.h)
set(HEADERS_FILES ${HEADERS_CONNECTION} ${HEADERS_DOCUMENT} ${HEADERS_GRAPHICS} ${HEADERS_EXCEPTIONS} ${HEADERS_SYSTEM} ${HEADERS_BASE})
# There is the C++ sources files.
set(SOURCES_CONNECTION src/connection/address.cpp)
set(SOURCES_DOCUMENT)
set(SOURCES_GRAPHICS src/graphics/color.cpp)
set(SOURCES_FILESYSTEM src/system/filesystem/filesystem.cpp)
set(SOURCES_SERIALIZERS)
set(SOURCES_SYSTEM ${SOURCES_FILESYSTEM} src/system/os.cpp src/system/terminal.cpp src/system/system.cpp src/system/uri.cpp)
set(SOURCES_BASE src/lambdacommon.cpp src/serializable.cpp src/lstring.cpp src/path.cpp src/utils.cpp src/resources.cpp)
set(SOURCES_FILES ${SOURCES_CONNECTION} ${SOURCES_DOCUMENT} ${SOURCES_GRAPHICS} ${SOURCES_SERIALIZERS} ${SOURCES_SYSTEM} ${SOURCES_BASE})

# Now build the library
# Build static if the option is on.
if (LAMBDACOMMON_BUILD_STATIC)
    add_library(lambdacommon_static STATIC ${HEADERS_FILES} ${SOURCES_FILES})
endif ()
# Build the shared library
add_library(lambdacommon SHARED ${HEADERS_FILES} ${SOURCES_FILES})
# Generate the export header and include it.
GENERATE_EXPORT_HEADER(lambdacommon
        BASE_NAME lambdacommon
        EXPORT_MACRO_NAME LAMBDACOMMON_API
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/lambdacommon_exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
target_include_directories(lambdacommon PUBLIC ${CMAKE_BINARY_DIR}/exports)
if (LAMBDACOMMON_BUILD_STATIC)
    target_compile_definitions(lambdacommon_static PUBLIC -DSHARED_EXPORTS_BUILT_AS_STATIC)
    target_include_directories(lambdacommon_static PUBLIC ${CMAKE_BINARY_DIR}/exports)
endif ()

# Set the variables
set(LAMBDACOMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Install if the option is on.
if (LAMBDACOMMON_INSTALL)
    foreach (LOOP_HEADER ${HEADERS_FILES})
        get_filename_component(HEADER_PARENT_DIR ${LOOP_HEADER} DIRECTORY)
        install(FILES ${LOOP_HEADER} DESTINATION ${HEADER_PARENT_DIR})
    endforeach ()
    install(FILES ${CMAKE_BINARY_DIR}/exports/lambdacommon_exports.h DESTINATION include/lambdacommon)
    install(TARGETS lambdacommon
            RUNTIME DESTINATION bin COMPONENT libraries
            LIBRARY DESTINATION lib COMPONENT libraries
            ARCHIVE DESTINATION lib COMPONENT libraries)
    if (LAMBDACOMMON_BUILD_STATIC)
        install(TARGETS lambdacommon_static
                RUNTIME DESTINATION bin COMPONENT libraries
                LIBRARY DESTINATION lib COMPONENT libraries
                ARCHIVE DESTINATION lib COMPONENT libraries)
    endif ()

    # Uninstall target
    if(NOT TARGET uninstall)
        configure_file(
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY)

        add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
    endif()

endif ()

# Build the tests if the option is on.
if (LAMBDACOMMON_BUILD_TESTS)
    add_subdirectory(tests)
endif ()
