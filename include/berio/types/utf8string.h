#ifndef BERIO_TYPES_UTF8STRING_H
#define BERIO_TYPES_UTF8STRING_H

#include <berio/io.h>

namespace ber {

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
tag_decode_utf8_string_result tag_decode_utf8_string(UTF8StrT& u8str, octet_input& in) {
  // TODO Implement
  return tdu8sr_unimplemented;
}

// TODO sizeof/encode UTF-8 string

} // end namespace ber

#endif // BERIO_TYPES_UTF8STRING_H
