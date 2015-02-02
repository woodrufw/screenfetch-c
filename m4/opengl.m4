# Author: Aaron Caffrey
# OPENGL_TEST() function in configure.ac
#
# Check for the presence of OpenGL headers and
# substitute the linker flags -lGL to the
# the variable 'GL_LIBS' if they are available
# Like the rest of screenfetch-c, this file is licensed under the MIT license.
AC_DEFUN([OPENGL_TEST],
[
  USE_GL="no"
  GL_LIBS=""

  AC_CHECK_LIB([GL], [glXChooseVisual], [
    AC_CHECK_HEADERS([GL/glx.h GL/gl.h], [
      GL_LIBS="-lGL"
      AC_SUBST(GL_LIBS)
      USE_GL="yes"
    ])
  ])

  AC_MSG_CHECKING([for the presence of GL headers])
  if test "$USE_GL" = "yes"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi
])