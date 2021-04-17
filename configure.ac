#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([cdist-cg],[0.1],[cdist@dtnr.ch])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CC

AC_LANG([C])

# Checks for libraries.

# Checks for header files.

dnl check stdlib.h, stdarg.h, string.h, and float.h
AC_HEADER_STDC

AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.

# Add debug support
AC_ARG_ENABLE(
	[debug],
	AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
	[case ${enableval} in
		(yes)	debug=true ;;
		(no)	debug=false ;;
		(*)	AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
	 esac],
	[debug=false])
AM_CONDITIONAL(DEBUG, [test "x${debug}" = xtrue])
AM_COND_IF(DEBUG,
	AC_DEFINE(DEBUG, 1, [Define to 0 if this is a release build]),
	AC_DEFINE(DEBUG, 0, [Define to 1 or higher if this is a debug build]))

AC_OUTPUT
