#ifndef BERIO_TYPES_OCTETSTRING_H
#define BERIO_TYPES_OCTETSTRING_H

#include <algorithm>
#include <iterator>

#include <berio/io.h>

namespace ber {

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
    
  static size_type size(OctetStrT const& ostr) noexcept { return ostr.size(); }
  
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
tag_decode_octet_string_result tag_decode_octet_string(OctetStrT& ostr, octet_input& in) {
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
std::size_t tag_sizeof_octet_string(OctetStrT const& ostr) noexcept {
  return Traits::to_size_t(Traits::size(ostr));
}

/*!
 * \ingroup builtin-types-octetstring
 */
template<typename OctetStrT, typename Traits = octet_string_traits<OctetStrT>>
void tag_encode_octet_string(OctetStrT const& ostr, octet_output& out) {
  using value_type = typename Traits::value_type;
  std::for_each(Traits::begin(ostr), Traits::end(ostr), [&out](value_type v) {
    out.put(Traits::to_octet(v));
  });
}

} // end namespace ber

#endif // BERIO_TYPES_OCTETSTRING_H
