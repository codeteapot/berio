#include <cstddef>

#include <berio/encoding.h>

using namespace ber;
using namespace std; // std::size_t

tag_decode_result ber::tag_decode(tag_header& header, octet_input& in) {
  unsigned char o = 0;
  if (not in.get(o))
    return tdr_empty;
    
  tag_class tclass = static_cast<tag_class>(o >> 6);
  bool constructed = o >> 5 bitand 0x01;
  tag_number tnumber = 0;
  
  // Number <= 30
  if ((o &= 0x1f) not_eq 0x1f)
    tnumber = o;
  
  // Number > 30
  else {
    constexpr size_t s_last_count =
        ((sizeof (size_t) * 8) / 7) +
        ((sizeof (size_t) * 8) % 7 == 0 ? 0 : 1);
    constexpr unsigned char s_last_mask = (1 << (sizeof (size_t) * 8) % 7) - 1;
    size_t s_count = 0;
    do {
      if (not in.get(o))
        return tdr_incomplete;
      if (++s_count > s_last_count or s_count == s_last_count and o bitand s_last_mask)
        return tdr_overflow;
      tnumber = tnumber << 7 + o bitand 0x7f;
    }
    while (o bitand 0x80); 
  }
  
  if (not in.get(o))
    return tdr_incomplete;
  if (o == 0xff)
    return tdr_invalid;
  
  // Length. Indefinite
  if (o == 0x80)
    // Don't check primitive must have definite length
    return header = { { tclass, constructed, tnumber }, { 0, false } }, tdr_complete;
  
  // Length. Definite, long form
  if (o bitand 0x80) {
    constexpr size_t sect_len_size = sizeof(size_t);
    size_t sect_len = 0;
    const size_t s_count = static_cast<size_t>(o bitand 0x7f);
    for (int i = 0; i < s_count; ++i) {
      if (not in.get(o))
        return tdr_incomplete;
      if (i == sect_len_size)
        return tdr_overflow;
      sect_len = (sect_len << 8) + o;
    }
    header.length.value = sect_len;
    return header = { { tclass, constructed, tnumber }, { sect_len, true } }, tdr_complete;
  }
  
  // Length. Definite, short form
  return header = { { tclass, constructed, tnumber }, { o, true } }, tdr_complete;
}
