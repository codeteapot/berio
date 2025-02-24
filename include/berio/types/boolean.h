#ifndef BERIO_TYPES_BOOLEAN_H
#define BERIO_TYPES_BOOLEAN_H

#include <berio/io.h>

namespace ber {

/*!
 * \ingroup builtin-types-boolean
 */
template<typename BoolT>
struct boolean_traits {

  static bool to_bool(BoolT const& b) { return static_cast<bool>(b); }
  
  static void assign(BoolT& b, bool bv) { b = bv; }
};

/*!
 * \ingroup builtin-types-boolean
 */
enum tag_decode_boolean_result {

  tdbr_complete,
  tdbr_empty
};

/*!
 * \ingroup builtin-types-boolean
 */
template<typename BoolT, typename Traits = boolean_traits<BoolT>>
tag_decode_boolean_result tag_decode_boolean(BoolT& b, octet_input& in) {
  unsigned char o;
  if (not in.get(o))
    return tdbr_empty;
  Traits::assign(b, o not_eq 0);
  return tdbr_complete;
}

/*!
 * \ingroup builtin-types-boolean
 */
template<typename BoolT, typename Traits = boolean_traits<BoolT>>
std::size_t tag_sizeof_boolean(BoolT const& b) noexcept {
  return 1;
}

/*!
 * \ingroup builtin-types-boolean
 */
template<typename BoolT, typename Traits = boolean_traits<BoolT>>
void tag_encode_boolean(BoolT const& b, octet_output& out) {
  out.put(Traits::to_bool(b) ? 0xff : 0x00); // DER restriction X.690 11.1
}

} // end namespace ber

#endif // BERIO_TYPES_BOOLEAN_H
