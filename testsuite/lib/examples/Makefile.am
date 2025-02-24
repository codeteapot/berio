noinst_PROGRAMS = \
  example-tag-decode \
  example-tag-sizeof \
  example-tag-shape

example_tag_decode_CPPFLAGS = \
  -I$(top_srcdir)/include

example_tag_decode_LDADD = \
  $(top_srcdir)/src/libberio.la
  
example_tag_decode_SOURCES = \
  tag-decode.cxx
  
example_tag_sizeof_CPPFLAGS = \
  -I$(top_srcdir)/include

example_tag_sizeof_LDADD = \
  $(top_srcdir)/src/libberio.la
  
example_tag_sizeof_SOURCES = \
  tag-sizeof.cxx

example_tag_shape_CPPFLAGS = \
  -I$(top_srcdir)/include

example_tag_shape_LDADD = \
  $(top_srcdir)/src/libberio.la

example_tag_shape_SOURCES = \
  tag-shape.cxx
  
CLEANFILES = $(AX_ENABLE_COVERAGE_CLEANFILES)
