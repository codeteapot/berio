noinst_PROGRAMS = \
  test-io.bin \
  test-tag.bin \
  test-type-boolean.bin

test_tag_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

test_tag_bin_LDADD = \
  $(top_srcdir)/src/libberio.la

test_tag_bin_SOURCES = \
  arrayio.h \
  test-tag.cxx
  
test_io_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

test_io_bin_LDADD = \
  $(top_srcdir)/src/libberio.la

test_io_bin_SOURCES = \
  tstub.h \
  test-io.cxx
  
test_type_boolean_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

test_type_boolean_bin_LDADD = \
  $(top_srcdir)/src/libberio.la

test_type_boolean_bin_SOURCES = \
  arrayio.h \
  test-type-boolean.cxx
  
CLEANFILES = $(AX_ENABLE_COVERAGE_CLEANFILES)
