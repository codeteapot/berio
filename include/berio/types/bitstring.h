#ifndef BERIO_TYPES_BITSTRING_H
#define BERIO_TYPES_BITSTRING_H

#include <berio/io.h>

namespace ber {

/*!
 * \ingroup builtin-types-bitstring
 */
template<typename BitStrT>
struct bit_string_traits {

  static void push_back(BitStrT& bstr, bool b) { bstr.push_back(b); }
};

/*!
 * \ingroup builtin-types-bitstring
 */
enum tag_decode_bit_string_result {

  tdbsr_complete,
  tdbsr_empty
};

/*!
 * \ingroup builtin-types-bitstring
 */
template<typename BitStringT, typename Traits = bit_string_traits<BitStringT>>
tag_decode_bit_string_result tag_decode_bit_string(BitStringT& bstr, octet_input& in) {
  unsigned char o;
  if (not in.get(o))
    return tdbsr_empty;
  // TODO Apply padding
  do {
    unsigned char b = o;
    for (int i = 0; i < 8; ++i) {
      Traits::push_back(bstr, (b & 0x80) >> 7);
      b << 1;
    }
  }
  while (in.get(o));
  return tdbsr_complete;
}

// TODO sizeof/encode bit string

} // end namespace ber

#endif // BERIO_TYPES_BITSTRING_H
