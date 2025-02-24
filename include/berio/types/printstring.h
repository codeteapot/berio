#ifndef BERIO_TYPES_PRINTSTRING_H
#define BERIO_TYPES_PRINTSTRING_H

#include <berio/io.h>

namespace ber {

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
tag_decode_print_string_result tag_decode_print_string(PrintStrT& pstr, octet_input& in) {
  unsigned char o;
  if (not in.get(o))
    return tdpsr_empty;
  do
    Traits::push_back(pstr, Traits::to_char(o));
  while (in.get(o));
  return tdpsr_complete;
}

// TODO sizeof/encode print string

} // end namespace ber

#endif // BERIO_TYPES_PRINTSTRING_H
