dnl AX_ENABLE_DEBUG([default])

AC_DEFUN([AX_ENABLE_DEBUG], [
  AC_ARG_ENABLE(
    [debug],
    [AS_HELP_STRING([--enable-debug], [compile with debugging flags (-g -O0)])],
    [debug_enabled=yes],
    [debug_enabled=[$1]])
    
  if test "x$debug_enabled" = "xyes"
  then
    CXXFLAGS="-g -O0"
    CPPFLAGS=""
    AC_MSG_NOTICE([Debugging enabled: CXXFLAGS='${CXXFLAGS}'])
  else
    AC_MSG_NOTICE([Debugging disabled or not specified])
  fi
])
