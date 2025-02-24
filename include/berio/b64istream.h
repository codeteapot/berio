#ifndef BERIO_B64ISTREAM_H
#define BERIO_B64ISTREAM_H

#include <cctype>
#include <istream>

#include <berio/b64.h>

namespace ber {

/*!
 * \ingroup in-out-base64-stdlib
 */
template<typename CharT, typename Traits>
struct b64_source_traits<std::basic_istream<CharT, Traits>> {

  static bool peek(std::basic_istream<CharT, Traits>& src, char& c) {
    typename Traits::int_type i = src.peek();
    if (i == Traits::eof())
      return false;
    c = src.narrow(Traits::to_char_type(i), '\0');
    return true;
  }
  
  static void ignore(std::basic_istream<CharT, Traits>& src) { src.ignore(); }
};

} // end namespace ber

#endif // BERIO_B64ISTREAM_H
