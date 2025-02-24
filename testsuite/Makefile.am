SUBDIRS = \
  config \
  lib \
  unit.tests \
  examples.tests
	
EXTRA_DIST = \
  global-conf.exp

AUTOMAKE_OPTIONS = dejagnu

@AX_DEJAGNU_EXPORT@

#DEJATOOL = unit examples
DEJATOOL = unit

CLEANFILES = $(AX_DEJAGNU_CLEANFILES)
