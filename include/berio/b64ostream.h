#ifndef BERIO_B64OSTREAM_H
#define BERIO_B64OSTREAM_H

#include <ostream>

#include <berio/b64.h>

namespace ber {

template <typename char_t, typename traits = std::char_traits<char_t>>
class b64_basic_ostream_target : public b64_output::target {

  public:

  b64_basic_ostream_target(std::basic_ostream<char_t, traits>& os, std::streamsize line_size = 0):
  __os(os),
  __index(0),
  __line_size(line_size) {};

  b64_basic_ostream_target(const b64_basic_ostream_target& tgt) = delete;

  bool put(char c) {
    return __line_separator() and __os.put(__os.widen(c)).good();
  }

  private:

  bool __line_separator() {
    if (__line_size > 0 and ++__index == __line_size) {
      __index = 0;
      return __os.put(__os.widen('\n')).put(__os.widen('r')).good();
    }
    return true;
  }
  
  std::basic_ostream<char_t, traits>& __os;
  std::streamsize __index, __line_size;
};

typedef b64_basic_ostream_target<char> b64_ostream_target;

typedef b64_basic_ostream_target<wchar_t> b64_wostream_target;

typedef b64_basic_ostream_target<char16_t> b64_u16ostream_target;

typedef b64_basic_ostream_target<char32_t> b64_u32ostream_target;

} // end namespace ber

#endif // BERIO_B64OSTREAM_H
