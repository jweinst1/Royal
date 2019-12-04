cmake_minimum_required(VERSION 3.1)

project(royal VERSION 0.0.2)

# Option to install targets to env location
if(DEFINED ENV{ROYAL_HOME})
     message("-- Will install ROYAL_HOME binaries and libraries to location: " $ENV{ROYAL_HOME})
	 set(CMAKE_INSTALL_PREFIX $ENV{ROYAL_HOME})
else(DEFINED ENV{ROYAL_HOME})
     message("-- Will install ROYAL_HOME binaries and libraries to location: " ${CMAKE_CURRENT_BINARY_DIR})
	 set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif(DEFINED ENV{ROYAL_HOME})


# Disable some windows specific warnings for using more modern c functions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

find_file(HAVE_STDINT_HEADER stdint.h)
if(HAVE_STDINT_HEADER)
   message("** stdint.h found")
   add_definitions(-DROYAL_HAVE_STDINT_HEADER)
else(HAVE_STDINT_HEADER)
   message("** stdint.h not found")
endif(HAVE_STDINT_HEADER)

# Options
option(WITH_testing "Allows tests to be run" ON)

if(WITH_testing MATCHES ON)
   enable_testing()
endif(WITH_testing MATCHES ON)

add_subdirectory(src)
