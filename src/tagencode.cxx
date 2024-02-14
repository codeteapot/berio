#include <berio/encoding.h>

using namespace ber;
using namespace std; // std::size_t

void ber::tag_encode(tag_header const& header, octet_output& out) {
  unsigned char o = (header.ident.tclass << 6) bitor (header.ident.constructed ? 0x20 : 0x00);
  
  tag_number const tnumber = header.ident.tnumber;
  if (tnumber <= 30)
    out.put(o bitor tnumber);
  else {
    out.put(0x1f);
    // TODO xxx1 1111 1xxx xxxx 0xxx xxx
    o = (tnumber >> (32 - 7 * 4)) bitand 0x7f; // BITOR 0x80. Discard if zero
    o = (tnumber >> (32 - 7 * 3)) bitand 0x7f; // BITOR 0x80. Discard if zero
    o = (tnumber >> (32 - 7 * 2)) bitand 0x7f; // BITOR 0x80. Discard if zero
    o = (tnumber >> (32 - 7 * 1)) bitand 0x7f; // BITOR 0x80. Discard if zero
    o = (tnumber >> (32 - 7 * 0)) bitand 0x7f;
  }
}
