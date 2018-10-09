#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libmpl], 0.1.4, iblackangel@163.com)
AC_CONFIG_SRCDIR([src/mapplication.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

lt_current="1"
lt_revision="4"
lt_age="1"
LTLDFLAGS="-version-info ${lt_current}:${lt_revision}:${lt_age}"
AC_SUBST(LTLDFLAGS)
 
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR
AC_PROG_LIBTOOL

# Checks for libraries.
# Add bellow content if you need to link with another library
# PKG_CHECK_MODULES([HELLO], [libhello >= 1.0])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h stdint.h stdlib.h string.h sys/socket.h syslog.h unistd.h fcntl.h inttypes.h limits.h netinet/in.h sys/file.h sys/ioctl.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([ftruncate gettimeofday memset socket strtoul])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
		 test/Makefile])
AC_CONFIG_FILES([libmpl.pc])
AC_OUTPUT
