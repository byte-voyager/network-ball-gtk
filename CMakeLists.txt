cmake_minimum_required(VERSION 3.10)
project(network-ball-gtk C)

set(CMAKE_C_STANDARD 99)

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})


# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(network-ball-gtk application.c)
TARGET_LINK_LIBRARIES(network-ball-gtk ${GTK3_LIBRARIES})
