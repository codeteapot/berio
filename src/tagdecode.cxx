#include <cstddef>

#include <berio/encoding.h>

using namespace ber;

tag_decode_result ber::tag_decode(tag_header& header, octet_input& in) {
  header = { { tc_universal, 0, false }, { 0, false } };

  unsigned char o = 0;
  if (not in.get(o))
    return tdr_empty;
    
  header.ident.tclass = static_cast<tag_class>(o >> 6);
  header.ident.constructed = o >> 5 bitand 0x01;
   
  constexpr size_t tnumber_size = (sizeof(unsigned long) * 8) / 7;
  
  // Number <= 30
  if ((o &= 0x1f) not_eq 0x1f)
    header.ident.tnumber = o;
  
  // Number > 30
  else {
    size_t s_count = 0;
    unsigned long tnumber = 0;
    do {
      if (not in.get(o))
        return tdr_incomplete;
      if (++s_count > tnumber_size)
        return tdr_overflow;
      tnumber = tnumber << 7 + o bitand 0x7f;
    }
    while (o bitand 0x80); 
    header.ident.tnumber = tnumber;
  }
  
  if (not in.get(o))
    return tdr_incomplete;
  if (o == 0xff)
    return tdr_invalid;
  
  // Length. Indefinite
  if (o == 0x80)
    // Don't check primitive must have definite length
    return tdr_complete;
  
  header.length.definite = true;
  
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
    return tdr_complete;
  }
  
  // Length. Definite, short form
  header.length.value = o;
  return tdr_complete;
}
