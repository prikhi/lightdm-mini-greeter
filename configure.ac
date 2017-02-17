#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([lightdm-mini-greeter], [0.1.2], [http://github.com/prikhi])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AM_INIT_AUTOMAKE([subdir-objects])

# Checks for libraries.
PKG_CHECK_MODULES(GTK, gtk+-3.0 >= 3.14)
PKG_CHECK_MODULES(LIGHTDM, liblightdm-gobject-1 >= 1.12)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
