#ifndef BERIO_B64OSTREAM_H
#define BERIO_B64OSTREAM_H

/*!
 * \file
 *
 * \brief Base-64 output adapter for the standard output character stream
 *
 * It contains the concept of derivation of std::basic_ostream and the traits that cover it.
 */

#include <ostream>

#include <berio/b64.h>

namespace ber {

/*!
 * \brief Everything that derives from a standard output character stream
 *
 * That is, any type derived from std::basic_ostream. So it includes
 *
 * - std::basic_ostringstream,
 * - std::basic_ofstream,
 * - std::basic_iostream,
 * 
 * and so on.
 *
 * \tparam T  Evaluated type.
 *
 * \see ber::b64_sink_traits<StdOutputStreamT>
 *
 * \ingroup in-out-base64-stdlib
 */
template<typename T>
concept StdOutputStream = std::derived_from<
  T,
  std::basic_ostream<typename T::char_type, typename T::traits_type>>;

/*!
 * \brief Traits for an octet output for Base-64 over a standard character output stream
 *
 * Implement \c put by using unformatted output operations on the sink whose type derives from
 * std::basic_ostream.
 *
 * \tparam StdOutputStreamT  Sink type. Requires satisfying ber::StdOutputStream.
 *
 * \see ber::b64_sink_traits
 *
 * \ingroup in-out-base64-stdlib
 */
template<StdOutputStream StdOutputStreamT>
struct b64_sink_traits<StdOutputStreamT> {

  /*!
   * \brief Insert a character into the sink
   *
   * Using std::basic_ostream::put(), it inserts the character \p c into \p sink, with its value
   * obtained via the std::basic_ios::widen() operation on \p sink.
   *
   * Determine whether the character was successfully inserted by checking the result of the call to
   * std::basic_ios::good() on \p sink.
   *
   * \param sink  Sink into which the character is inserted.
   * \param c     The value of the character to be inserted.
   *
   * \return \c true if the character was successfully inserted; \c false otherwise.
   */
  static bool put(StdOutputStreamT& sink, char c) {
    return sink.put(sink.widen(c)).good();
  }
};

} // end namespace ber

#endif // BERIO_B64OSTREAM_H
