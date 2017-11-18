cmake_minimum_required (VERSION 3.2)
project (lightdm-mini-greeter
    VERSION 0.2.0)

# Set compiler flags
set(CMAKE_C_FLAGS "-march=native -mtune=native -O3 --std=c11")

# Use the package PkgConfig to find libraries and headers
find_package(PkgConfig REQUIRED)

# Detect GTK+ headers/library files
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Find lightdm library and headers
pkg_check_modules(LIGHTDM REQUIRED liblightdm-gobject-1>=1.2.1)

# Setup CMake to use GTK+ and lightdm, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS} ${LIGHTDM_INCLUDE_DIRS} include)
link_directories(${GTK3_LIBRARY_DIRS} ${LIGHTDM_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

# Get all sources files
file(GLOB_RECURSE SOURCES src/*.c)

# Create the executable
add_executable (lightdm-mini-greeter ${SOURCES})
# And link the libraries
target_link_libraries(lightdm-mini-greeter ${GTK3_LIBRARIES} ${LIGHTDM_LIBRARIES})

# Install the executable and the other files
install(TARGETS lightdm-mini-greeter RUNTIME DESTINATION /usr/bin)
install(FILES data/lightdm-mini-greeter.desktop DESTINATION /usr/share/xgreeters)
install(FILES data/lightdm-mini-greeter.conf DESTINATION /etc/lightdm)
