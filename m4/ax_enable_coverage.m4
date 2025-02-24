dnl AX_ENABLE_COVERAGE([default])

AC_DEFUN([AX_ENABLE_COVERAGE], [
  AC_ARG_ENABLE([coverage],
    AS_HELP_STRING([--enable-coverage], [Enable code coverage via gcov]),
    [],
    [enable_coverage=[$1]])

  AS_IF([test "x$enable_coverage" = "xyes"], [
    AC_MSG_CHECKING([lcov and genhtml])
    AS_IF([(command -v lcov && command -v genhtml) >/dev/null >&1], [
      AC_MSG_RESULT([yes])
      CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"
      CXXFLAGS="$CXXFLAGS -fprofile-arcs -ftest-coverage"
      LDFLAGS="$LDFLAGS -fprofile-arcs -ftest-coverage"
      AX_ENABLE_COVERAGE_snippet="
coverage-init:
	lcov --directory . --zerocounters

coverage.info: coverage-init check
	lcov --rc geninfo_unexecuted_blocks=1 --directory . --capture --output-file coverage.info
	lcov --remove coverage.info \$(AX_ENABLE_COVERAGE_REMOVEFILES) --output-file coverage.info
	
coverage-report: coverage.info
	genhtml coverage.info --output-directory coverage-report
	
.PHONY: coverage-init coverage-report
"
        AC_DEFINE([CODE_COVERAGE_ENABLED], [1], [Define if coverage is enabled])
      ], [
        AC_MSG_RESULT([no])
        exit 1
      ])
    ], [
      AX_ENABLE_COVERAGE_snippet=""
    ])
    
  AC_SUBST([AX_ENABLE_COVERAGE_REMOVEFILES], ["'/usr/*' '*/tests/*'"])
  AC_SUBST([AX_ENABLE_COVERAGE_CLEANFILES], ["coverage.info coverage-report *.gcda *.gcno"])
  AC_SUBST([AX_ENABLE_COVERAGE_RULES], ["${AX_ENABLE_COVERAGE_snippet}"])
  AM_SUBST_NOTMAKE([AX_ENABLE_COVERAGE_RULES])
])
