include $(top_srcdir)/doc/config/doxygen.am

pkgconfigdir = $(datadir)/pkgconfig

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = \
  include \
  src \
  testsuite

EXTRADIST = \
  AUTHORS \
  ChangeLog \
  NEWS \
  README \
  TODO \
  m4/doxygen.m4

dist_pkgconfig_DATA = \
  berio.pc

README: README.md
	cat $< > README
