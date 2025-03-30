if(LINUX)
	target_compile_definitions(game
		PRIVATE
		OS_LINUX
	)

	find_package(PkgConfig REQUIRED)
	find_package(Threads REQUIRED)

	# Wayland/X11 detection
	pkg_check_modules(WAYLAND
		QUIET
			wayland-client
			wayland-cursor
			xkbcommon
	)
	pkg_check_modules(X11
		QUIET
			x11
			xext
	)

	# Security hardening
	target_compile_options(game
		PRIVATE
			#-D_FORTIFY_SOURCE=2
			-fstack-protector-strong
	)
	target_link_options(game
		PRIVATE
			-Wl,-z,relro,-z,now
			-Wl,-Bsymbolic-functions
	)

	# Link dependencies
	target_link_libraries(game
		PRIVATE
			Threads::Threads
			${WAYLAND_LINK_LIBRARIES}
			${X11_LINK_LIBRARIES}
	)

	# Conditional compilation
	target_compile_definitions(game
		PRIVATE
			$<$<BOOL:${WAYLAND_FOUND}>:HAS_WAYLAND>
			$<$<BOOL:${X11_FOUND}>:HAS_X11>
	)

	set_target_properties(game
		PROPERTIES
			POSITION_INDEPENDENT_CODE ON
	)
elseif(WIN32)
	target_compile_definitions(game
		PRIVATE
			OS_WIN
	)

	set_target_properties(game
		PROPERTIES
			WIN32_EXECUTABLE TRUE
	)
endif()
