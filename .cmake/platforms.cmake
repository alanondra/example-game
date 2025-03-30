# https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html#variables-that-describe-the-system

if(NOT(LINUX OR WIN32))
	message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(FATAL_ERROR "Unsupported architecture")
endif()
