dnl AX_DEJAGNU[global-conf-rel-path]

AC_DEFUN([AX_DEJAGNU], [
  AS_IF([test "x$DEJAGNU" = "x"], [
    AC_SUBST([DEJAGNU], ["\$(top_srcdir)/$1"])
    AC_SUBST([AX_DEJAGNU_CLEANFILES], ["site.exp site.bak *.log *.sum"])
    AC_SUBST([AX_DEJAGNU_EXPORT], ["export DEJAGNU"])
    AM_SUBST_NOTMAKE([AX_DEJAGNU_EXPORT])
  ])
])
