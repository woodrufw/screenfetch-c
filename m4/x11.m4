# Author: Aaron Caffrey
# TEST_X11() function in configure.ac
#
# Check for the presence of X11 headers and
# substitute the linker flags -lX11 to the
# the variable 'X_LIBS' if they are available
# Like the rest of screenfetch-c, this file is licensed under the MIT license.
AC_DEFUN([TEST_X11],
[
  X_LIBS=""

  AC_CHECK_HEADERS([X11/Xlib.h X11/Xatom.h], [
    X_LIBS="-lX11"
    AC_SUBST(X_LIBS)
  ])

])