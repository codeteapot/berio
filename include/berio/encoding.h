#ifndef BERIO_ENCODING_H
#define BERIO_ENCODING_H

#include <berio/io.h>
#include <berio/tag.h>

namespace ber {

enum tag_decode_result {

  tdr_complete,
  tdr_empty,
  tdr_incomplete,
  tdr_overflow,
  tdr_invalid
};

std::size_t tag_sizeof(tag_ident const& ident);

std::size_t tag_sizeof(tag_length const& length);

tag_decode_result tag_decode(tag_header& header, octet_input& in);

void tag_encode(tag_header const& header, octet_output& out);

} // end namespace ber

#endif // BERIO_ENCODING_H
