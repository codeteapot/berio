AC_INIT([berio],[0.1])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  doc/Makefile
  include/Makefile
  include/berio/Makefile
  src/Makefile
  testsuite/Makefile
  testsuite/config/Makefile
  testsuite/lib/Makefile
  testsuite/lib/bertree/Makefile
  testsuite/bertree.tests/Makefile
])

AM_INIT_AUTOMAKE([-Wall -Werror])

AC_PROG_CXX
AM_PROG_AR

AX_ENABLE_DEBUG([no])
AX_DEJAGNU([testsuite/global-conf.exp])

LT_INIT

DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg],[api])
DX_DOXYGEN_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

AC_OUTPUT
