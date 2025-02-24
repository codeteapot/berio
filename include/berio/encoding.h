#ifndef BERIO_ENCODING_H
#define BERIO_ENCODING_H

#include <berio/io.h>
#include <berio/tag.h>

namespace ber {

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

std::size_t tag_sizeof(tag_header const& th);

tag_decode_result tag_decode(tag_header& th, octet_input& in);

void tag_encode(tag_header const& th, octet_output& out);

} // end namespace ber

#endif // BERIO_ENCODING_H
