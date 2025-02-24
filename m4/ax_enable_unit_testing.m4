dnl AX_ENABLE_UNIT_TESTING([yes/no],[varname])

AC_DEFUN([AX_ENABLE_UNIT_TESTING], [
  AC_ARG_ENABLE([unit-testing],
    AS_HELP_STRING([--enable-unit-testing], [Enable unit testing extensions]),
    [],
    [enable_unit_testing=[$1]])
    
  AM_CONDITIONAL([ENABLE_UNIT_TESTING], [test "x$enable_unit_testing" = "xyes"])
  
  AS_IF([test "x$enable_unit_testing" = "xyes"], [
    CPPFLAGS="$CPPFLAGS -D[$2]"
  ])
])
