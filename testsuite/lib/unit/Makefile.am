noinst_PROGRAMS = \
  test-tag \
  test-type-boolean

test_tag_CPPFLAGS = \
  -I$(top_srcdir)/include

test_tag_LDADD = \
  $(top_srcdir)/src/libberio.la

test_tag_SOURCES = \
  testio.h \
  test-tag.cxx
  
test_type_boolean_CPPFLAGS = \
  -I$(top_srcdir)/include

test_type_boolean_LDADD = \
  $(top_srcdir)/src/libberio.la

test_type_boolean_SOURCES = \
  testio.h \
  test-type-boolean.cxx
  
CLEANFILES = $(AX_ENABLE_COVERAGE_CLEANFILES)
