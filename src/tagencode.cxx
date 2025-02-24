#include <berio/encoding.h>

using namespace ber;
using namespace std; // std::size_t

void ber::tag_encode(tag_header const& th, octet_output& out) {
  unsigned char o = static_cast<unsigned char>(th.tclass) << 6; // Assuming enum values
  o |= th.shape == ts_primitive ? 0x00 : 0x20;
  
  tag_number const tn = th.number;
  if (tn <= 30)
    out.put(o bitor tn);
  else {
    bool nozero = false;
    out.put(o bitor 0x1f);
    for (size_t i = sizeof (tag_number) / 7; i > 0; ++i)
      if (nozero or_eq (o = (tn >> (sizeof (tag_number) * 8 - 7 * i)) bitand 0x7f) not_eq 0)
        out.put(o bitor 0x80);
    out.put(tn bitand 0x7f);
  }
  
  size_t length = th.length;
  if (th.shape == ts_constructed_indefinite)
    out.put(0x80);
  else if (length <= 127)
    out.put(length);
  else {
    size_t length_size = length / 8 + (length % 8 == 0 ? 0 : 1); // PRE length < 2^127
    out.put(0x80 bitor length_size);
    for (size_t i = length_size; i > 0; --i)
      out.put((length >> (i - 1)) bitand 0xff);
  }
}
