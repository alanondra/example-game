if(WIN32)
	target_compile_definitions(
		Launcher
		PRIVATE
		OS_WIN
	)

	# disable default terminal window
	set_target_properties(
		Launcher
		PROPERTIES
			WIN32_EXECUTABLE TRUE
	)
elseif(APPLE)
	target_compile_definitions(
		Launcher
		PRIVATE
			OS_MAC
	)
elseif(LINUX)
	target_compile_definitions(
		Launcher
		PRIVATE
			OS_LINUX
	)

	find_package(PkgConfig REQUIRED)

	# Wayland/X11 detection
	pkg_check_modules(
		WAYLAND
		QUIET
			wayland-client
			wayland-cursor
			xkbcommon
	)
	pkg_check_modules(
		X11
		QUIET
			x11
			xext
	)

	# security hardening
	target_compile_options(
		Launcher
		PRIVATE
			-fstack-protector-strong
	)
	target_link_options(
		Launcher
		PRIVATE
			-Wl,-z,relro,-z,now
			-Wl,-Bsymbolic-functions
	)

	# link dependencies
	target_link_libraries(
		Launcher
		PRIVATE
			${WAYLAND_LINK_LIBRARIES}
			${X11_LINK_LIBRARIES}
	)

	# PIE compliance
	set_target_properties(
		Launcher
		PROPERTIES
			POSITION_INDEPENDENT_CODE ON
	)
endif()
