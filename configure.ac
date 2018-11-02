#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([lightdm-mini-greeter], [0.3.4], [http://github.com/prikhi])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([defines.h])

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

# Check if liblightdm is v1.19.1 or below for debian builds
LDM_CHECK=$(pkg-config --max-version=1.19.1 liblightdm-gobject-1)
AS_IF([test $? = 0],
      [AC_DEFINE([LIGHTDM_1_19_1_LOWER], [], [Defined if liblightdm is version 1.19.1 or lower])]
      )

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
