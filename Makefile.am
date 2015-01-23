# Global Options
AUTOMAKE_OPTIONS = \
				foreign # Disable requirement for README, AUTHORS, COPYING

AM_CFLAGS = -g -Wall -O3 --std=c11 -Wextra -Werror --std=c11 -pedantic \
             -Wno-unused-parameter -Wfloat-equal -Wundef -Wshadow      \
             -Wpointer-arith -Wcast-align -Wstrict-prototypes          \
             -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return    \
             -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2  \
             -Winit-self                                               \
             -ftrapv -fverbose-asm


# Packaging
EXTRA_DIST = \
			autogen.sh

DISTCLEANFILES = \
			aclocal.m4

MAINTAINERCLEANFILES = \
					Makefile.in \
					compile \
					config.h.in \
					configure \
					depcomp \
					install-sh \
					missing


# Data Files
xgreetersdir = $(datadir)/xgreeters
dist_xgreeters_DATA = lightdm-mini-greeter.desktop


# Greeter Executable
greeter_PROGRAMS = lightdm-mini-greeter
greeterdir = $(bindir)

lightdm_mini_greeter_SOURCES = \
							src/main.c \
							src/callbacks.o \
							src/utils.o

lightdm_mini_greeter_CFLAGS = \
							$(GTK_CFLAGS) \
							$(LIGHTDM_CFLAGS)
lightdm_mini_greeter_LDFLAGS = \
							$(GTK_LIBS) \
							$(LIGHTDM_LIBS)
