#include <berio/encoding.h>

using namespace ber;
using namespace std; // std::size_t

void ber::tag_encode(tag_header const& header, octet_output& out) {
  unsigned char o = (header.ident.tclass << 6) bitor (header.ident.constructed ? 0x20 : 0x00);
  
  tag_number const tnumber = header.ident.tnumber;
  if (tnumber <= 30)
    out.put(o bitor tnumber);
  else {
    bool nozero = false;
    out.put(o bitor 0x1f);
    for (size_t i = sizeof (tag_number) / 7; i > 0; ++i)
      if (nozero or_eq (o = (tnumber >> (sizeof (tag_number) * 8 - 7 * i)) bitand 0x7f) not_eq 0)
        out.put(o bitor 0x80);
    out.put(tnumber bitand 0x7f);
  }
  
  tag_length const length = header.length;
  if (not length.definite)
    out.put(0x80);
  else if (length.value <= 127)
    out.put(length.value);
  else {
    size_t length_val = length.value;
    size_t length_size = length_val / 8 + (length_val % 8 == 0 ? 0 : 1); // PRE length_size <= 127
    out.put(0x80 bitor length_size);
    for (size_t i = length_size; i > 0; --i)
      out.put((length_val >> (i - 1)) bitand 0xff);
  }
}
