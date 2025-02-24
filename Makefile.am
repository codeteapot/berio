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
	
AX_ENABLE_COVERAGE_REMOVEFILES = '/usr/*' '*/tests/*' '*/testsuite/*'

@AX_ENABLE_COVERAGE_RULES@
@DX_RULES@

SOURCE_FORMAT_DIRS = include src testsuite/lib/bertree

source-format:
	find $(SOURCE_FORMAT_DIRS) -iname '*.cxx' -o -iname '*.h' | xargs clang-format -i

.PHONY: source-format

CLEANFILES = $(DX_CLEANFILES) $(AX_ENABLE_COVERAGE_CLEANFILES)
