SUBDIRS = \
  config \
  lib \
  unit.tests \
  bertree.tests \
  examples.tests
	
EXTRA_DIST = \
  global-conf.exp

AUTOMAKE_OPTIONS = dejagnu

@AX_DEJAGNU_EXPORT@

#DEJATOOL = unit bertree examples
DEJATOOL = unit

CLEANFILES = $(AX_DEJAGNU_CLEANFILES)
