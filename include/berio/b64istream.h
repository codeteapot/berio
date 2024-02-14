#ifndef BERIO_B64ISTREAM_H
#define BERIO_B64ISTREAM_H

#include <cctype>
#include <istream>

#include <berio/b64.h>

namespace ber {

template <typename char_t, typename traits = std::char_traits<char_t>>
class b64_basic_istream_source : public b64_input::source {

  public:

  b64_basic_istream_source(std::basic_istream<char_t, traits>& is):
  __is(is) {};

  b64_basic_istream_source(const b64_basic_istream_source& src) = delete;

  bool peek(char& c) {
    typename traits::int_type i = __is.peek();
    while (i not_eq traits::eof()) {
      c = __is.narrow(traits::to_char_type(i), 0);
      if (not isspace(c))
        return true;
      __is.ignore();
      i = __is.peek();
    }
    return false;
  }

  void ignore() { __is.ignore(); }

  private:

  std::basic_istream<char_t, traits>& __is;
};

typedef b64_basic_istream_source<char> b64_istream_source;

typedef b64_basic_istream_source<wchar_t> b64_wistream_source;

typedef b64_basic_istream_source<char16_t> b64_u16istream_source;

typedef b64_basic_istream_source<char32_t> b64_u32istream_source;

} // end namespace ber

#endif // BERIO_B64ISTREAM_H
