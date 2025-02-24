dnl AX_DEJAGNU

AC_DEFUN([AX_DEJAGNU], [
  if test "x$DEJAGNU" = "x"
  then
    DEJAGNU="\$(top_srcdir)/$1"
  fi
  
  AC_SUBST(DEJAGNU)
])
