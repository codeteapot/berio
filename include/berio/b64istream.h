#ifndef BERIO_B64ISTREAM_H
#define BERIO_B64ISTREAM_H

#include <istream>

#include <berio/b64.h>

namespace ber {

/*!
 * \ingroup in-out-base64-stdlib
 */
template<typename T>
concept StdInputStream = std::derived_from<
  T,
  std::basic_istream<typename T::char_type, typename T::traits_type>>;

/*!
 * \ingroup in-out-base64-stdlib
 */
template<StdInputStream SourceT>
struct b64_source_traits<SourceT> {

  static bool peek(SourceT& src, char& c) {
    using traits_type = typename SourceT::traits_type;
    
    typename traits_type::int_type i = src.peek();
    if (i == traits_type::eof())
      return false;
    c = src.narrow(traits_type::to_char_type(i), '\0');
    return true;
  }
  
  static void ignore(SourceT& src) { src.ignore(); }
};

} // end namespace ber

#endif // BERIO_B64ISTREAM_H
