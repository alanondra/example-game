project(common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_MODULE_EXTENSIONS .cppm)

include(packages.cmake)
include(sources.cmake)

add_library(Common STATIC)

target_include_directories(
	Common
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
	Common
	PRIVATE
	#standard
	Threads::Threads
)

target_sources(
	Common
	PUBLIC
		FILE_SET CXX_MODULES
		FILES ${MODULES}
)

target_compile_features(
	Common
	PUBLIC
		cxx_std_23
)
