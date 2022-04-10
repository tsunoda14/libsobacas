#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([libsobacas], [0.1])
AC_CONFIG_SRCDIR([winscard-wrapper.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AM_INIT_AUTOMAKE([foreign])

AC_ARG_ENABLE([install-headers],
    [AS_HELP_STRING([--enable-install-headers],
        [enable installing the cloned PC/SC headers])],
    [], [enable_install_headers=no])
AM_CONDITIONAL([INSTALL_PCSC_HEADERS], [test x$enable_install_headers != xno])

AC_ARG_VAR([YAKISOBA_CFLAGS], [C compiler flags for libyakisoba (ex. -I/foo/bar)])
AC_ARG_VAR([YAKISOBA_LIBS], [linker flags to use libyakisoba (ex. -L/foo -lbar)])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

save_libs="$LIBS"
LIBS="$YAKISOBA_LIBS $LIBS"
AC_SEARCH_LIBS([bcas_decodeECM], [yakisoba])
if test x"$ac_cv_search_bcas_decodeECM" != x'no'; then
    if test x"$ac_cv_search_bcas_decodeECM" != x'none required'; then
        YAKISOBA_LIBS="$YAKISOBA_LIBS $ac_cv_search_bcas_decodeECM"
    fi
else
    AC_MSG_ERROR(
[cannot find the library libyakisoba.

Consider adjusting the YAKISOBA_LIBS environment variable
if you installed the library in a non-standard prefix.
])
fi
LIBS="$save_libs"

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

AC_DEFINE_UNQUOTED([orig_cfflags], ["$CFLAGS"])
AC_DEFINE_UNQUOTED([CFLAGS], ["$CFLAGS $YAKISOBA_CFLAGS"])
AC_CHECK_HEADERS([yakisoba.h], [],
    [AC_MSG_ERROR(
[cannot find the header file yakisoba.h.

Consider adjusting the YAKISOBA_CFLAGS environment variable
if you installed the header in a non-standard prefix.
])
    ])
AC_DEFINE([CFLAGS], [orig_cfflags])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([select])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
