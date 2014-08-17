#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([kuru-kuru-rin], [0.1], [aabe.65535@gmail.com])
AC_CONFIG_SRCDIR([src/kkr-main.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
PKG_CHECK_MODULES([Gtkmm], [gtkmm-2.4 >= 2.20.3])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_RESTRICT

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
