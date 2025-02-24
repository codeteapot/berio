#ifndef BERIO_B64SSTREAM_H
#define BERIO_B64SSTREAM_H

#include <fstream>

#include <berio/b64istream.h>
#include <berio/b64ostream.h>

namespace ber {

/*!
 * \ingroup in-out-base64-stdlib
 */
template<typename CharT, typename Traits>
struct b64_source_traits<std::basic_fstream<CharT, Traits>> :
  b64_source_traits<std::basic_istream<CharT, Traits>> {
};

/*!
 * \ingroup in-out-base64-stdlib
 */
template<typename CharT, typename Traits>
struct b64_sink_traits<std::basic_fstream<CharT, Traits>> :
  b64_sink_traits<std::basic_ostream<CharT, Traits>> {
};


} // end namespace ber

#endif // BERIO_B64SSTREAM_H
