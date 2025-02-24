#include <cstddef>
#include <limits>

#include <berio/tag.h>

using namespace ber;
using namespace std;

tag_decode_result ber::tag_decode(tag_header& th, octet_input& in, bool strict) {
  unsigned char o = 0;
  if (not in.get(o))
    return tdr_empty;
    
  tag_class const tc = static_cast<tag_class>(o >> 6); // Assuming enum values
  bool const constr = o >> 5 bitand 0x01;
  
  tag_number tn;
  // Number <= 30
  if ((o &= 0x1f) not_eq 0x1f)
    tn = o;
  // Number > 30
  else {
    constexpr tag_number tag_number_limit = numeric_limits<tag_number>::max() >> 7;
    // First octet
    if (not in.get(o))
      return tdr_number_incomplete;
    if (o == 0 and strict)
      return tdr_number_zero_leading;
    tn = o bitand 0x7f;
    // Following octets
    while (o bitand 0x80) {
      if (tn > tag_number_limit)
        return tdr_number_overflow;
      if (not in.get(o))
        return tdr_number_incomplete;
      tn = (tn << 7) bitor (o bitand 0x7f);
    }
    if (tn <= 30 and strict)
      return tdr_number_unneeded_high;
  }
  
  if (not in.get(o))
    return tdr_length_undefined;
  if (o == 0xff)
    return tdr_length_unsupported;
  
  tag_shape ts = ts_constructed_indefinite;
  size_t len = 0;
  // Length. Indefinite
  if (o == 0x80) {
    if (not constr)
      return tdr_indefinite_primitive;
  }
  // Length. Definite
  else {
    ts = constr ? ts_constructed_definite : ts_primitive;
    
    // Long form
    if (o bitand 0x80) {
      constexpr std::size_t size_t_limit = numeric_limits<size_t>::max() >> 8;
      size_t const count = static_cast<size_t>(o bitand 0x7f);
      for (int i = 0; i < count; ++i) {
        if (not in.get(o))
          return tdr_length_incomplete;
        if (len > size_t_limit)
          return tdr_length_overflow;
        len = (len << 8) bitor o;
      }
      if (len <= 127 and strict)
        return tdr_length_unneeded_long;
    }
    // Short form
    else
      len = o;
  }
  
  th = { tc, tn, ts, len };
  return tdr_complete;
}
