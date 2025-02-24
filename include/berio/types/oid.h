#ifndef BERIO_TYPES_OID_H
#define BERIO_TYPES_OID_H

#include <berio/io.h>

namespace ber {

/*!
 * \ingroup builtin-types-oid
 */
template<typename OIDT, typename CompT = unsigned long, std::size_t CompSize = sizeof (CompT)>
struct oid_traits {

  typedef CompT comp_type;
  
  constexpr static std::size_t comp_size = CompSize;
  
  constexpr static CompT comp_zero() { return {}; }
  
  static CompT int_to_comp(int i) { return static_cast<CompT>(i); }
  
  static bool comp_lt(CompT comp1, CompT comp2) { return comp1 < comp2; }
  
  static CompT comp_minus(CompT comp1, CompT comp2) { return comp1 - comp2; }
  
  static void comp_l_shift(CompT& comp, int n) { comp = comp << n; }
  
  static void comp_bitor_assign(CompT& comp, unsigned char o) { comp |= o; }
  
  static void push_back(OIDT& oid, CompT comp) { oid.push_back(comp); }
};

/*!
 * \ingroup builtin-types-oid
 */
enum tag_decode_oid_result {

  tdoidr_complete,
  tdoidr_empty,
  tdoidr_incomplete,
  tdoidr_overflow
};

/*!
 * \ingroup builtin-types-oid
 */
template<typename OIDT, typename Traits = oid_traits<OIDT>>
tag_decode_oid_result tag_decode_oid(OIDT& oid, octet_input& in) {
  constexpr std::size_t comp_bsize = Traits::comp_size * 8;
  unsigned char o;
  if (not in.get(o))
    return tdoidr_empty;
  typename Traits::comp_type comp = Traits::comp_zero();
  std::size_t bsize = 14;
  while (o bitand 0x80) {
    if (bsize > comp_bsize)
      return tdoidr_overflow;
    Traits::comp_l_shift(comp, 7);
    Traits::comp_bitor_assign(comp, o bitand 0x7f);
    if (not in.get(o))
      return tdoidr_incomplete;
    bsize += 7;
  }
  Traits::comp_l_shift(comp, 7);
  Traits::comp_bitor_assign(comp, o bitand 0x7f);
  if (Traits::comp_lt(comp, Traits::int_to_comp(40))) {
    Traits::push_back(oid, Traits::int_to_comp(0));
    Traits::push_back(oid, comp);
  }
  else if (Traits::comp_lt(comp, Traits::int_to_comp(80))) {
    Traits::push_back(oid, Traits::int_to_comp(1));
    Traits::push_back(oid, Traits::comp_minus(comp, Traits::int_to_comp(40)));
  }
  else {
    Traits::push_back(oid, Traits::int_to_comp(2));
    Traits::push_back(oid, Traits::comp_minus(comp, Traits::int_to_comp(80)));
  }
  while (in.get(o)) {
    comp = Traits::comp_zero();
    bsize = 14;
    while (o bitand 0x80) {
      if (bsize > comp_bsize)
        return tdoidr_overflow;
      Traits::comp_l_shift(comp, 7);
      Traits::comp_bitor_assign(comp, o bitand 0x7f);
      if (not in.get(o))
        return tdoidr_incomplete;
      bsize += 7;
    }
    Traits::comp_l_shift(comp, 7);
    Traits::comp_bitor_assign(comp, o bitand 0x7f);
    Traits::push_back(oid, comp);
  }
  return tdoidr_complete;
}

// TODO sizeof/encode OID

} // end namespace ber

#endif // BERIO_TYPES_OID_H
