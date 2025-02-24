#ifndef BERIO_B64OSTREAM_H
#define BERIO_B64OSTREAM_H

#include <ostream>

#include <berio/b64.h>

namespace ber {

/*!
 * \ingroup in-out-base64-stdlib
 */
template<typename T>
concept StdOutputStream = std::derived_from<
  T,
  std::basic_ostream<typename T::char_type, typename T::traits_type>>;

/*!
 * \ingroup in-out-base64-stdlib
 */
template<StdOutputStream SinkT>
struct b64_sink_traits<SinkT> {

  static bool put(SinkT& sink, char c) {
    return sink.put(sink.widen(c)).good();
  }
};

} // end namespace ber

#endif // BERIO_B64OSTREAM_H
