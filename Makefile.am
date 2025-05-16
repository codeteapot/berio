#include $(top_srcdir)/doc/config/doxygen.am
include $(top_srcdir)/debian.am

pkgconfigdir = $(datadir)/pkgconfig

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = \
  doc \
  include \
  src \
  testsuite

EXTRADIST = \
  AUTHORS \
  ChangeLog \
  NEWS \
  README \
  TODO

dist_pkgconfig_DATA = \
  berio.pc

README: README.md
	cat $< >README
	
TODO: TODO.md
	cat $< >TODO
