#ifndef BERIO_B64OSTREAM_H
#define BERIO_B64OSTREAM_H

#include <ostream>

#include <berio/b64.h>

namespace ber {

/*!
 * \ingroup in-out-base64-stdlib
 */
template<typename CharT, typename Traits>
struct b64_sink_traits<std::basic_ostream<CharT, Traits>> {

  static bool put(std::basic_ostream<CharT, Traits>& sink, char c) {
    return sink.put(sink.widen(c)).good();
  }
  
  static void flush(std::basic_ostream<CharT, Traits>& sink) {
    sink.flush();
  }
};

} // end namespace ber

#endif // BERIO_B64OSTREAM_H
