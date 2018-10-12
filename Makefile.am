# Global Options
AUTOMAKE_OPTIONS = \
				foreign # Disable requirement for README, AUTHORS, COPYING

AM_CFLAGS = -g -Wall -O3 --std=c11 -Wextra -Werror --std=c11 -pedantic \
			-Wno-unused-parameter -Wfloat-equal -Wundef -Wshadow      \
			-Wpointer-arith -Wcast-align -Wstrict-prototypes          \
			-Wstrict-overflow=5 -Wwrite-strings -Waggregate-return    \
			-Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2  \
			-Winit-self                                               \
			-ftrapv -fverbose-asm \
			-DCONFIG_FILE=\""$(sysconfdir)/lightdm/lightdm-mini-greeter.conf"\"


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
dist_xgreeters_DATA = data/lightdm-mini-greeter.desktop

configdir = $(sysconfdir)/lightdm
dist_config_DATA = data/lightdm-mini-greeter.conf


# Greeter Executable
greeterdir = $(bindir)
greeter_PROGRAMS = lightdm-mini-greeter

lightdm_mini_greeter_SOURCES = \
							src/main.c \
							src/app.c \
							src/callbacks.c \
							src/compat.c \
							src/config.c \
							src/ui.c \
							src/utils.c

lightdm_mini_greeter_CFLAGS = \
							$(AM_CFLAGS) \
							$(GTK_CFLAGS) \
							$(LIGHTDM_CFLAGS)
lightdm_mini_greeter_LDADD = \
							$(GTK_LIBS) \
							$(LIGHTDM_LIBS)
