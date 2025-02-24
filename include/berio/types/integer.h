#ifndef BERIO_TYPES_INTEGER_H
#define BERIO_TYPES_INTEGER_H

#include <berio/io.h>

namespace ber {

/*!
 * \ingroup builtin-types-integer
 */
template<typename IntT, std::size_t IntSize = sizeof (IntT)>
struct integer_traits {

  constexpr static std::size_t integer_size = IntSize;
  
  constexpr static IntT zero() { return {}; }
  
  static void l_shift(IntT& i, int n) { i = i << n; }
  
  static void bitor_assign(IntT& i, unsigned char o) { i |= o; }
};

/*!
 * \ingroup builtin-types-integer
 */
enum tag_decode_integer_result {

  tdir_complete,
  tdir_empty,
  tdir_overflow
};

/*!
 * \ingroup builtin-types-integer
 */
template<typename IntT, typename Traits = integer_traits<IntT>>
tag_decode_integer_result tag_decode_integer(IntT& i, octet_input& in) {
  i = Traits::zero();
  unsigned char o;
  if (not in.get(o))
    return tdir_empty;
  Traits::bitor_assign(i, o);
  std::size_t size = 0;
  while (in.get(o)) {
    if (++size > Traits::integer_size)
      return tdir_overflow;
    Traits::l_shift(i, 8);
    Traits::bitor_assign(i, o);
  }
  return tdir_complete;
}

// TODO sizeof/encode integer

} // end namespace ber

#endif // BERIO_TYPES_INTEGER_H
