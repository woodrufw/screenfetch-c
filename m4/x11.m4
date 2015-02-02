# Author: Aaron Caffrey
# X11_TEST() function in configure.ac
#
# Check for the presence of X11 headers and
# substitute the linker flags -lX11 to the
# the variable 'X_LIBS' if they are available
# Like the rest of screenfetch-c, this file is licensed under the MIT license.
AC_DEFUN([X11_TEST],
[
  USE_X="no"
  X_LIBS=""

  AC_CHECK_LIB([X11], [XOpenDisplay], [
    AC_CHECK_HEADERS([X11/Xlib.h X11/Xatom.h], [
      X_LIBS="-lX11"
      AC_SUBST(X_LIBS)
      USE_X="yes"
    ])
  ])

  AC_MSG_CHECKING([for the presence of X11 headers])
  if test "$USE_X" = "yes"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])