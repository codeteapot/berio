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
  TODO

dist_pkgconfig_DATA = \
  berio.pc

README: README.md
	cat $< >README
	
TODO: TODO.md
	cat $< >TODO
	
@DX_RULES@

CLEANFILES = $(DX_CLEANFILES)
