#ifndef BERIO_B64ISTREAM_H
#define BERIO_B64ISTREAM_H

/*!
 * \file
 *
 * \brief Base-64 input adapter for the standard input character stream
 *
 * It contains the concept of derivation of std::basic_istream and the traits that cover it.
 */

#include <istream>

#include <berio/b64.h>

namespace ber {

/*!
 * \brief Everything that derives from a standard input character stream
 *
 * That is, any type derived from std::basic_istream. So it includes
 *
 * - std::basic_istringstream,
 * - std::basic_ifstream,
 * - std::basic_iostream,
 * 
 * and so on.
 *
 * \tparam T  Evaluated type.
 *
 * \see ber::b64_source_traits<StdInputStreamT>
 *
 * \ingroup in-out-base64-stdlib
 */
template<typename T>
concept StdInputStream = std::derived_from<
  T,
  std::basic_istream<typename T::char_type, typename T::traits_type>>;

/*!
 * \brief Traits for an octet input for Base-64 over a standard character input stream
 *
 * Implement \c peek and \c ignore by using unformatted input operations on the source whose type
 * derives from std::basic_istream.
 *
 * \tparam StdInputStreamT  Source type. Requires satisfying ber::StdInputStream.
 *
 * \see ber::b64_source_traits
 *
 * \ingroup in-out-base64-stdlib
 */
template<StdInputStream StdInputStreamT>
struct b64_source_traits<StdInputStreamT> {

  /*!
   * \brief Reveal the next character from the source
   *
   * Retrieves the next character from \p src without discarding it, using
   * std::basic_istream::peek(). If a character is available, its value--as determined by the
   * std::basic_ios::narrow() operation on \p src --is assigned to \p c.
   *
   * A character is considered available if its value is different from std::char_traits::eof().
   *
   * \param src  Source from which the following character is revealed.
   * \param c    Reference to the character to which the obtained value is assigned.
   *
   * \return \c true if a character was available; \c false otherwise.
   */
  static bool peek(StdInputStreamT& src, char& c) {
    using traits_type = typename StdInputStreamT::traits_type;
    
    typename traits_type::int_type i = src.peek();
    if (i == traits_type::eof())
      return false;
    c = src.narrow(traits_type::to_char_type(i), '\0');
    return true;
  }
  
  /*!
   * \brief Discard the next character from the source
   *
   * Using the std::basic_istream::ignore() operation on \p src.
   *
   * \param src  Source whose next character is discarded.
   */
  static void ignore(StdInputStreamT& src) { src.ignore(); }
};

} // end namespace ber

#endif // BERIO_B64ISTREAM_H
