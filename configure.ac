AC_INIT([berio],[0.1])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  berio.pc
  Makefile
  doc/Doxyfile
  include/Makefile
  include/berio/Makefile
  src/Makefile
  testsuite/Makefile
  testsuite/config/Makefile
  testsuite/lib/Makefile
  testsuite/lib/unit/Makefile
  testsuite/lib/bertree/Makefile
  testsuite/lib/examples/Makefile
  testsuite/unit.tests/Makefile
  testsuite/bertree.tests/Makefile
  testsuite/examples.tests/Makefile
])

AM_INIT_AUTOMAKE([-Wall -Werror])

AX_CHECK_ENABLE_DEBUG([no])
AX_ENABLE_COVERAGE([no])
AX_DEJAGNU([testsuite/global-conf.exp])

AX_CXX_COMPILE_STDCXX([20], [noext], [mandatory])

AC_PROG_CXX
AM_PROG_AR

LT_INIT

DX_INIT_DOXYGEN([$PACKAGE_NAME],[doc/Doxyfile],[doc/output])
DX_DOXYGEN_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(ON)
DX_PS_FEATURE(OFF)

AC_OUTPUT
