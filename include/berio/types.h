#ifndef BERIO_TYPES_H
#define BERIO_TYPES_H

#include <algorithm>
#include <iterator>

#include <berio/io.h>
#include <berio/tag.h>

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
std::size_t tag_sizeof_boolean(BoolT const& b) {
  return 1;
}

/*!
 * \ingroup builtin-types-boolean
 */
template<typename BoolT, typename Traits = boolean_traits<BoolT>>
void tag_encode_boolean(octet_output& out, BoolT const& b) {
  out.put(Traits::to_bool(b) ? 0xff : 0x00); // DER restriction X.690 11.1
}

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
tag_decode_integer_result tag_decode_integer(octet_input& in, IntT& i) {
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
tag_decode_bit_string_result tag_decode_bit_string(octet_input& in, BitStringT& bstr) {
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

/*!
 * \ingroup builtin-types-octetstring
 */
template<typename OctetStrT>
struct octet_string_traits {

  using size_type = typename OctetStrT::size_type;
  using value_type = typename OctetStrT::value_type;
  using const_iterator = typename OctetStrT::const_iterator;
  
  static std::size_t to_size_t(size_type s) { return static_cast<std::size_t>(s); }
  
  static unsigned char to_octet(value_type v) { return static_cast<unsigned char>(v); }
    
  static size_type size(OctetStrT const& ostr) { return ostr.size(); }
  
  static const_iterator begin(OctetStrT const& ostr) { return std::begin(ostr); }
  
  static const_iterator end(OctetStrT const& ostr) { return std::end(ostr); }
  
  static void push_back(OctetStrT& ostr, unsigned char o) { ostr.push_back(o); }
};

/*!
 * \ingroup builtin-types-octetstring
 */
enum tag_decode_octet_string_result {

  tdosr_complete,
  tdosr_empty
};

/*!
 * \ingroup builtin-types-octetstring
 */
template<typename OctetStrT, typename Traits = octet_string_traits<OctetStrT>>
tag_decode_octet_string_result tag_decode_octet_string(octet_input& in, OctetStrT& ostr) {
  unsigned char o;
  if (not in.get(o))
    return tdosr_empty;
  do
    Traits::push_back(ostr, o);
  while (in.get(o));
  return tdosr_complete;
}

/*!
 * \ingroup builtin-types-octetstring
 */
template<typename OctetStrT, typename Traits = octet_string_traits<OctetStrT>>
std::size_t tag_sizeof_octet_string(OctetStrT const& ostr) {
  return Traits::to_size_t(Traits::size(ostr));
}

/*!
 * \ingroup builtin-types-octetstring
 */
template<typename OctetStrT, typename Traits = octet_string_traits<OctetStrT>>
void tag_encode_octet_string(octet_output& out, OctetStrT const& ostr) {
  using value_type = typename Traits::value_type;
  std::for_each(Traits::begin(ostr), Traits::end(ostr), [&out](value_type v) {
    out.put(Traits::to_octet(v));
  });
}

/*!
 * \ingroup builtin-types-utf8string
 */
template<typename UTF8StrT>
struct utf8_string_traits {

  static void push_back(UTF8StrT& u8str, char32_t c) { u8str.push_back(c); }
};

/*!
 * \ingroup builtin-types-utf8string
 */
enum tag_decode_utf8_string_result {

  tdu8sr_unimplemented
};

/*!
 * \ingroup builtin-types-utf8string
 */
template<typename UTF8StrT, typename Traits = utf8_string_traits<UTF8StrT>>
tag_decode_utf8_string_result tag_decode_utf8_string(octet_input& in, UTF8StrT& u8str) {
  // TODO Implement
  return tdu8sr_unimplemented;
}

// TODO sizeof/encode UTF string

/*!
 * \ingroup builtin-types-printstring
 */
template<typename PrintStrT>
struct print_string_traits {

  static char to_char(unsigned char o) { return static_cast<char>(o); }
  
  static void push_back(PrintStrT& pstr, char c) { pstr.push_back(c); }
};

/*!
 * \ingroup builtin-types-printstring
 */
enum tag_decode_print_string_result {

  tdpsr_complete,
  tdpsr_empty
};

/*!
 * \ingroup builtin-types-printstring
 */
template<typename PrintStrT, typename Traits = print_string_traits<PrintStrT>>
tag_decode_print_string_result tag_decode_print_string(octet_input& in, PrintStrT& pstr) {
  unsigned char o;
  if (not in.get(o))
    return tdpsr_empty;
  do
    Traits::push_back(pstr, Traits::to_char(o));
  while (in.get(o));
  return tdpsr_complete;
}

// TODO sizeof/encode print string

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
tag_decode_oid_result tag_decode_oid(octet_input& in, OIDT& oid) {
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

#endif // BERIO_TYPES_H
