noinst_PROGRAMS = \
  example-is-eoc.bin \
  example-tag-decode.bin \
  example-tag-sizeof.bin \
  example-tag-encode.bin \
  example-tag-shape.bin

example_is_eoc_bin_CPPFLAGS = \
  -I$(top_srcdir)/include

example_is_eoc_bin_LDADD = \
  $(top_srcdir)/src/libberio.la
  
example_is_eoc_bin_SOURCES = \
  is-eoc.cxx
  
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
