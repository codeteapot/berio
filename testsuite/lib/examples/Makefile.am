noinst_PROGRAMS = \
  example-tag-decode.bin \
  example-tag-sizeof.bin \
  example-tag-encode.bin \
  example-tag-shape.bin

example_tag_decode_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

example_tag_decode_bin_LDADD = \
  $(top_srcdir)/src/libberio.la
  
example_tag_decode_bin_SOURCES = \
  tag-decode.cxx
  
example_tag_sizeof_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

example_tag_sizeof_bin_LDADD = \
  $(top_srcdir)/src/libberio.la
  
example_tag_sizeof_bin_SOURCES = \
  tag-sizeof.cxx
  
example_tag_encode_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

example_tag_encode_bin_LDADD = \
  $(top_srcdir)/src/libberio.la
  
example_tag_encode_bin_SOURCES = \
  tag-encode.cxx

example_tag_shape_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

example_tag_shape_bin_LDADD = \
  $(top_srcdir)/src/libberio.la

example_tag_shape_bin_SOURCES = \
  tag-shape.cxx
  
CLEANFILES = $(AX_ENABLE_COVERAGE_CLEANFILES)
