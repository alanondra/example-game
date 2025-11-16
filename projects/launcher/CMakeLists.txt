project(launcher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_MODULE_EXTENSIONS .cppm)

include(packages.cmake)
include(sources.cmake)

add_executable(Launcher ${SRC})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(Launcher PRIVATE DEBUG)
endif()

target_sources(
	Launcher
	PRIVATE
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES ${MODULES}
)

target_link_libraries(
	Launcher
	PRIVATE
		#standard
		Threads::Threads
		# internal
		Common
		Setup
		# external
		wx::base
		wx::core
)

target_compile_features(
	Launcher
	PRIVATE
	cxx_std_23
)

include(platforms.cmake)
