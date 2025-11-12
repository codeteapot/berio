#include <cstddef>

#include <berio/tag.h>

using namespace ber;
using namespace std; // std::size_t

tag_decode_result ber::tag_decode(tag_header& th, octet_input& in) {
  th = { tc_universal, 0, ts_primitive, 0 };
  
  unsigned char o = 0;
  if (not in.get(o))
    return tdr_empty;
    
  th.tclass = static_cast<tag_class>(o >> 6); // Assuming enum values
  bool const constr = o >> 5 bitand 0x01;
  
  // Number <= 30
  if ((o &= 0x1f) not_eq 0x1f)
    th.number = o;
  
  // Number > 30
  else {
    constexpr tag_number tag_number_max = static_cast<tag_number>(-1);
    th.number = 0;
    do {
      if (not in.get(o))
        return tdr_number_incomplete;
      int curr = o bitand 0x7f;
      // First should be non-zero: X.690-202101 8.1.2.4.2.c
      if (tag_number_max - th.number < curr)
        return tdr_number_overflow;
      th.number = (th.number << 7) + curr;
    }
    while (o bitand 0x80);
    // Resulting number should be > 30
  }
  
  if (not in.get(o))
    return tdr_length_undefined;
  if (o == 0xff)
    return tdr_length_unsupported; // X.690-0207 8.1.3.5.c
  
  // Length. Indefinite
  if (o == 0x80) {
    if (not constr)
      return tdr_indefinite_primitive;
    return th.shape = ts_constructed_indefinite, th.length = 0, tdr_complete;
  }
  
  th.shape = constr ? ts_constructed_definite : ts_primitive;
  
  // Length. Definite, long form
  if (o bitand 0x80) {
    constexpr std::size_t s_len_size = sizeof(std::size_t);
    std::size_t const s_count = static_cast<std::size_t>(o bitand 0x7f);
    std::size_t s_len = 0;
    for (int i = 0; i < s_count; ++i) {
      if (not in.get(o))
        return tdr_length_incomplete;
      if (i == s_len_size)
        return tdr_length_overflow;
      s_len = (s_len << 8) + o;
    }
    return th.length = s_len, tdr_complete;
  }
  
  // Length. Definite, short form
  return th.length = o, tdr_complete;
}
