#include <cstddef>

#include <berio/encoding.h>

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
    constexpr size_t s_last_count =
        ((sizeof (size_t) * 8) / 7) +
        ((sizeof (size_t) * 8) % 7 == 0 ? 0 : 1);
    constexpr unsigned char s_last_mask = (1 << (sizeof (size_t) * 8) % 7) - 1;
    size_t s_count = 0;
    do {
      if (not in.get(o))
        return tdr_number_incomplete;
      if (++s_count > s_last_count or s_count == s_last_count and o bitand s_last_mask)
        return tdr_number_overflow;
      th.number = th.number << 7 + o bitand 0x7f;
    }
    while (o bitand 0x80); 
  }
  
  if (not in.get(o))
    return tdr_length_undefined;
  if (o == 0xff)
    return tdr_length_unsupported; // X.690-0207 8.1.3.5.c
  
  // Length. Indefinite
  if (o == 0x80)
    return constr
      ? th.shape = ts_constructed_indefinite, th.length = 0, tdr_complete
      : tdr_indefinite_primitive;
  
  th.shape = constr ? ts_constructed_definite : ts_primitive;
  
  // Length. Definite, long form
  if (o bitand 0x80) {
    constexpr size_t s_len_size = sizeof(size_t);
    size_t s_len = 0;
    const size_t s_count = static_cast<size_t>(o bitand 0x7f);
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
