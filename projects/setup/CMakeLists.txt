project(setup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_MODULE_EXTENSIONS .cppm)

include(packages.cmake)
include(sources.cmake)

add_library(Setup STATIC)

target_include_directories(
	Setup
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(
	Setup
	PUBLIC
		FILE_SET CXX_MODULES
		FILES ${MODULES}
)

target_link_libraries(
	Setup
	PRIVATE
		# internal
		Common
)

target_compile_features(
	Setup
	PUBLIC
		cxx_std_23
)
