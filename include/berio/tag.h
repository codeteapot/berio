#ifndef BERIO_TAG_H
#define BERIO_TAG_H

#include <berio/io.h>

namespace ber {

enum tag_class {

  tc_universal = 0,
  tc_application = 1,
  tc_context_specific = 2,
  tc_private = 3
};

typedef unsigned int tag_number;

enum tag_shape {

  ts_primitive,
  ts_constructed_definite,
  ts_constructed_indefinite
};

struct tag_header {
  
  tag_class tclass;
  tag_number number;
  tag_shape shape;
  std::size_t length;
};

bool operator == (tag_header const&, tag_header const&);

constexpr tag_header th_end = { tc_universal, 0, ts_primitive, 0 };

enum tag_decode_result {

  tdr_complete,
  tdr_empty,
  tdr_indefinite_primitive,
  tdr_number_incomplete,
  tdr_number_overflow,
  tdr_length_undefined,
  tdr_length_unsupported,
  tdr_length_incomplete,
  tdr_length_overflow
};

tag_decode_result tag_decode(tag_header& th, octet_input& in);

std::size_t tag_sizeof(tag_header const& th);

void tag_encode(tag_header const& th, octet_output& out);

} // end namespace ber

#endif // BERIO_TAG_H
