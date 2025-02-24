#include <berio/tag.h>

using namespace ber;
using namespace std; // std::size_t

namespace {

void tag_encode_ltn(tag_number tn_rem, octet_output& out) {
  if (tn_rem == 0)
    return;
  tag_encode_ltn(tn_rem >> 7, out);
  out.put(tn_rem bitor 0x80);
}

void tag_encode_ltn_last(tag_number tn, octet_output& out) {
  tag_encode_ltn(tn >> 7, out);
  out.put(tn bitand 0x7f);
}

} // anonymous namespace

size_t ber::tag_sizeof(tag_header const& th) {
  size_t num_extra_size = 0;
  if (th.number > 30) {
    ++num_extra_size;
    for (size_t tn = th.number; tn >>= 7; tn > 0)
      ++num_extra_size;
  }
  size_t len_extra_size = 0;
  if (th.shape not_eq ts_constructed_indefinite and th.length > 127) {
    ++len_extra_size;
    for (size_t len = th.length; len >>= 8; len > 0)
      ++len_extra_size;
  }
  return 2 + num_extra_size + len_extra_size;
}

void ber::tag_encode(tag_header const& th, octet_output& out) {
  unsigned char o = static_cast<unsigned char>(th.tclass) << 6; // Assuming enum values
  o |= th.shape == ts_primitive ? 0x00 : 0x20;
  
  if (th.number <= 30)
    out.put(o bitor th.number);
  else {
    out.put(o bitor 0x1f);
    tag_encode_ltn_last(th.number, out);
  }
  
  size_t length = th.length;
  if (th.shape == ts_constructed_indefinite)
    out.put(0x80);
  else if (length <= 127)
    out.put(length);
  else {
    size_t length_size = (length + 7) / 8; // TODO Review: It seems a bad implementation
    out.put(0x80 bitor length_size);
    for (size_t i = length_size; i > 0; --i)
      out.put((length >> (i - 1)) bitand 0xff);
  }
}
