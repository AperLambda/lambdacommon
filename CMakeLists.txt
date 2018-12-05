cmake_minimum_required(VERSION 3.6)
project(lambdacommon)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(GenerateExportHeader)
include(utils/CompileUtility)
include(utils/InstallUtility)

set(CMAKE_CXX_STANDARD 14)

# Options
option(LAMBDACOMMON_BUILD_STATIC "Build static libraries" OFF)
option(LAMBDACOMMON_INSTALL "Generate installation target" ON)
option(LAMBDACOMMON_BUILD_TESTS "Build the λcommon test programs" ON)

generate_flags(LAMBDACOMMON_COMPILE_FLAGS "native" 2 true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${LAMBDACOMMON_COMPILE_FLAGS}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework CoreFoundation -framework ApplicationServices")
endif()

# All files:
# There is the C++ header files.
set(HEADERS_CONNECTION include/lambdacommon/connection/address.h)
set(HEADERS_DOCUMENT include/lambdacommon/documents/document.h)
set(HEADERS_GRAPHICS include/lambdacommon/graphics/color.h include/lambdacommon/graphics/scene.h)
set(HEADERS_MATHS include/lambdacommon/maths.h include/lambdacommon/maths/geometry/geometry.h)
set(HEADERS_FILESYSTEM include/lambdacommon/system/fs/filesystem.h)
set(HEADERS_EXCEPTIONS include/lambdacommon/exceptions/exceptions.h)
set(HEADERS_SYSTEM ${HEADERS_FILESYSTEM} include/lambdacommon/system/os.h include/lambdacommon/system/devices.h include/lambdacommon/system/terminal.h include/lambdacommon/system/system.h include/lambdacommon/system/uri.h include/lambdacommon/system/time.h)
set(HEADERS_BASE include/lambdacommon/lambdacommon.h include/lambdacommon/serializable.h include/lambdacommon/lstring.h include/lambdacommon/path.h include/lambdacommon/resources.h include/lambdacommon/sizes.h include/lambdacommon/types.h include/lambdacommon/object.h)
set(HEADER_FILES ${HEADERS_CONNECTION} ${HEADERS_DOCUMENT} ${HEADERS_GRAPHICS} ${HEADERS_MATHS} ${HEADERS_EXCEPTIONS} ${HEADERS_SYSTEM} ${HEADERS_BASE})
# There is the C++ source files.
set(SOURCES_CONNECTION src/connection/address.cpp)
set(SOURCES_DOCUMENT)
set(SOURCES_GRAPHICS src/graphics/color.cpp src/graphics/scene.cpp)
set(SOURCES_MATHS src/maths.cpp)
set(SOURCES_FILESYSTEM src/system/fs/filesystem.cpp)
set(SOURCES_SERIALIZERS)
set(SOURCES_SYSTEM ${SOURCES_FILESYSTEM} src/system/os.cpp src/system/terminal.cpp src/system/system.cpp src/system/uri.cpp src/system/time.cpp)
set(SOURCES_BASE src/lambdacommon.cpp src/serializable.cpp src/lstring.cpp src/path.cpp src/resources.cpp)
set(SOURCE_FILES ${SOURCES_CONNECTION} ${SOURCES_DOCUMENT} ${SOURCES_GRAPHICS} ${SOURCES_MATHS} ${SOURCES_SERIALIZERS} ${SOURCES_SYSTEM} ${SOURCES_BASE})

# Now build the library
# Build static if the option is on.
if (LAMBDACOMMON_BUILD_STATIC)
	add_library(lambdacommon_static STATIC ${HEADER_FILES} ${SOURCE_FILES})
endif ()
# Build the shared library
add_library(lambdacommon SHARED ${HEADER_FILES} ${SOURCE_FILES})
# Generate the export header and include it.
GENERATE_EXPORT_HEADER(lambdacommon
		BASE_NAME lambdacommon
		EXPORT_MACRO_NAME LAMBDACOMMON_API
		EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/lambdacommon_exports.h
		STATIC_DEFINE LAMBDACOMMON_SHARED_EXPORTS_BUILT_AS_STATIC)
target_include_directories(lambdacommon PUBLIC ${CMAKE_BINARY_DIR}/exports)
if (LAMBDACOMMON_BUILD_STATIC)
	target_compile_definitions(lambdacommon_static PUBLIC -DLAMBDACOMMON_SHARED_EXPORTS_BUILT_AS_STATIC)
	target_include_directories(lambdacommon_static PUBLIC ${CMAKE_BINARY_DIR}/exports)
endif ()

# Install if the option is on.
if (LAMBDACOMMON_INSTALL)
	foreach (LOOP_HEADER ${HEADER_FILES})
		get_filename_component(HEADER_PARENT_DIR ${LOOP_HEADER} DIRECTORY)
		install(FILES ${LOOP_HEADER} DESTINATION ${HEADER_PARENT_DIR})
	endforeach ()
	install(FILES ${CMAKE_BINARY_DIR}/exports/lambdacommon_exports.h DESTINATION include/lambdacommon)
	install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION share/licenses/lambdacommon)
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

	# Uninstall target.
	setup_uninstall_target()
endif ()

# Build the tests if the option is on.
if (LAMBDACOMMON_BUILD_TESTS)
	add_subdirectory(tests)
endif ()

add_subdirectory(c_wrapper)