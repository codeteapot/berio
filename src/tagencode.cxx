#include <berio/tag.h>

using namespace ber;
using namespace std; // std::size_t

namespace {

bool tag_encode_ltn(tag_number tn_rem, octet_output& out) {
  return tn_rem == 0 or tag_encode_ltn(tn_rem >> 7, out) and out.put(tn_rem bitor 0x80);
}

bool tag_encode_ltn_last(tag_number tn, octet_output& out) {
  return tag_encode_ltn(tn >> 7, out) and out.put(tn bitand 0x7f);
}

template<typename T>
size_t tag_sizeof_ext(T v, int bs) {
  size_t s = 1;
  for (T tv = v; tv >>= bs; tv > 0)
    ++s;
  return s;
}

} // anonymous namespace

size_t ber::tag_sizeof(tag_header const& th) noexcept {
  return 2 +
    (th.number <= 30 ? 0 : tag_sizeof_ext(th.number, 7)) +
    (th.shape == ts_constructed_indefinite or th.length <= 127 ? 0 : tag_sizeof_ext(th.length, 8));
}

bool ber::tag_encode(tag_header const& th, octet_output& out) {
  unsigned char o = static_cast<unsigned char>(th.tclass) << 6; // Assuming enum values
  o |= th.shape == ts_primitive ? 0x00 : 0x20;
  
  if (th.number <= 30) {
    if (not out.put(o bitor th.number))
      return false;
  }
  else {
    if (not out.put(o bitor 0x1f) or not tag_encode_ltn_last(th.number, out))
      return false;
  }
  
  size_t length = th.length;
  if (th.shape == ts_constructed_indefinite) {
    if (not out.put(0x80))
      return false;
  }
  else if (length <= 127) {
    if (not out.put(length))
      return false;
  }
  else {
    size_t length_size = tag_sizeof_ext(length, 8);
    if (not out.put(0x80 bitor length_size))
      return false;
    for (size_t i = length_size; i > 0; --i) {
      if (not out.put((length >> (i - 1)) bitand 0xff))
        return false;
    }
  }
  return true;
}
