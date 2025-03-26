project(game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_MODULE_EXTENSIONS .cppm)

include(dependencies.cmake)

set(SRC src/main.cpp)

include(modules.cmake)

add_executable(Game ${SRC})

target_sources(
	Game
	PRIVATE
	${SRC}
	PRIVATE
		FILE_SET cxx_modules
		TYPE CXX_MODULES
		FILES ${MODULES}
)

target_link_libraries(
	Game
	PRIVATE
		${DEPENDENCIES}
)

target_compile_features(
	Game
	PRIVATE
		cxx_std_23
)
