#ifndef BERIO_B64_H
#define BERIO_B64_H

#include <cctype>

#include <berio/io.h>

namespace ber {

/*!
 * \ingroup in-out-base64
 */
template<typename SourceT>
struct b64_source_traits {

  static bool peek(SourceT& src, char& c) {
    return false;
  }
  
  static void ignore(SourceT& src) {}
};

/*!
 * \ingroup in-out-base64
 */
template<typename SinkT>
struct b64_sink_traits {

  static bool put(SinkT& sink, char c) { return false; }
    
  static void flush(SinkT& sink) {}
};

/*!
 * \ingroup in-out-base64
 */
template<typename SourceT, typename Traits = b64_source_traits<SourceT>>
class b64_input : public octet_input {

  public:

  b64_input(SourceT& src) noexcept:
  __src(src),
  __last_s(0),
  __get_fn(&b64_input::__get_first) {}
  
  std::size_t get(unsigned char* optr, std::size_t len) {
    std::size_t actual_len = 0;
    while (actual_len < len and (this->*__get_fn)(optr + actual_len))
      ++actual_len;
    return actual_len;
  }
  
  std::size_t skip(std::size_t len) {
    unsigned char null;
    std::size_t actual_len = 0;
    while (actual_len < len and (this->*__get_fn)(&null))
      ++actual_len;
    return actual_len;
  }

  private:
  
  struct alphabet_sextet_table {
  
    static constexpr std::size_t array_size = 128;
    static constexpr unsigned char invalid_sextet = 0xc0;
    
    unsigned char array[array_size];
    
    constexpr alphabet_sextet_table():
    array() {
      for (std::size_t i = 0; i < array_size; ++i) {
        char c = static_cast<char>(i);
        if (c >= 'A' and c <= 'Z')
          array[i] = c - 'A';
        else if (c >= 'a' and c <= 'z')
          array[i] = c - 'a' + 26;
        else if (c >= '0' and c <= '9')
          array[i] = c - '0' + 52;
        else if (c == '+')
          array[i] = 62;
        else if (c == '/')
          array[i] = 63;
        else
          array[i] = invalid_sextet;
      }
    }
    
    unsigned char operator [] (char c) const {
      return c > 0 ? array[c] : invalid_sextet;
    }
  };
  
  static constexpr alphabet_sextet_table __as_table {};
  
  SourceT& __src;
  unsigned char __last_s;
  bool (b64_input::*__get_fn)(unsigned char*);
  
  void __to_first() {
    __last_s = 0; // Not strictly needed
    __get_fn = &b64_input::__get_first;
  }
  
  void __to_second(unsigned char last_s) {
    __last_s = last_s;
    __get_fn = &b64_input::__get_second;
  }
  
  void __to_third(unsigned char last_s) {
    __last_s = last_s;
    __get_fn = &b64_input::__get_third;
  }
  
  bool __get_first(unsigned char* optr) {
    char c;
    if (not __source_peek(c))
      return false;
    unsigned char const s1 = __as_table[c];
    if (s1 == alphabet_sextet_table::invalid_sextet) // Including '='
      return false;
    Traits::ignore(__src);
    if (not __source_peek(c))
      return false;
    unsigned char const s2 = __as_table[c];
    if (s2 == alphabet_sextet_table::invalid_sextet) // Including '='
      return false;
    Traits::ignore(__src);
    *optr = s1 << 2 bitor s2 >> 4;
    __to_second(s2);
    return true;
  }
  
  bool __get_second(unsigned char* optr) {
    char c;
    if (not __source_peek(c))
      return false;
    if (c == '=') {
      Traits::ignore(__src);
      Traits::ignore(__src); // Assumes that next character is '='
      return false;
    }
    unsigned char const s = __as_table[c];
    if (s == alphabet_sextet_table::invalid_sextet)
      return false;
    Traits::ignore(__src);
    *optr = __last_s << 4 bitor s >> 2;
    __to_third(s);
    return true;
  }
  
  bool __get_third(unsigned char* optr) {
    char c;
    if (not __source_peek(c))
      return false;
    if (c == '=') {
      Traits::ignore(__src);
      return false;
    }
    unsigned char const s = __as_table[c];
    if (s == alphabet_sextet_table::invalid_sextet) // Including '='
      return false;
    Traits::ignore(__src);
    *optr = __last_s << 6 bitor s;
    __to_first();
    return true;
  }
  
  bool __source_peek(char& c) {
    while (Traits::peek(__src, c)) {
      if (not std::isspace(c))
        return true;
      Traits::ignore(__src);
    }
    return false;
  }
};

/*!
 * \ingroup in-out-base64
 */
template<typename SinkT, typename Traits = b64_sink_traits<SinkT>>
class b64_output : public octet_output {

  public:

  b64_output(SinkT& sink, std::size_t line_size = 0) noexcept:
  __sink(sink),
  __line_size(line_size),
  __index(0),
  __last_o(0), // Not actually needed
  __put_fn(&b64_output::__put_first),
  __conclude_fn(&b64_output::__conclude_first) {}

  std::size_t put(unsigned char const* optr, std::size_t len) {
    std::size_t actual_len = 0;
    while (actual_len < len and (this->*__put_fn)(optr + actual_len))
      ++actual_len;
    return actual_len;
  }
  
  void flush() {
    (this->*__conclude_fn)();
    Traits::flush(__sink);
  }

  private:
  
  struct sextet_alphabet_table {
  
    static constexpr std::size_t array_size = 64;
    
    unsigned char array[array_size];
    
    constexpr sextet_alphabet_table():
    array() {
      for (std::size_t i = 0; i < array_size; ++i) {
        unsigned char s = static_cast<unsigned char>(i);
        if (s >= 0 and s <= 25)
          array[i] = s + 'A';
        else if (s >= 26 and s <= 51)
          array[i] = s - 26 + 'a';
        else if (s >= 52 and s <= 61)
          array[i] = s - 52 + '0';
        else if (s == 62)
          array[i] = '+';
        else
          array[i] = '/'; // PRE s < 64, so s = 63 here
      }
    }
    
    unsigned char operator [] (unsigned char s) const { // PRE s < 64
      return array[s];
    }
  };
  static constexpr sextet_alphabet_table __sa_table {};
  
  SinkT& __sink;
  std::size_t const __line_size;
  std::size_t __index;
  unsigned char __last_o;
  bool (b64_output::*__put_fn)(unsigned char const*);
  void (b64_output::*__conclude_fn)();
  
  void __to_first() {
    // __last_o = 0
    __put_fn = &b64_output::__put_first;
    __conclude_fn = &b64_output::__conclude_first;
  }
  
  void __to_second(unsigned char last_o) {
    __last_o = last_o;
    __put_fn = &b64_output::__put_second;
    __conclude_fn = &b64_output::__conclude_second;
  }
  
  void __to_third(unsigned char last_o) {
    __last_o = last_o;
    __put_fn = &b64_output::__put_third;
    __conclude_fn = &b64_output::__conclude_third;
  }
  
  bool __put_first(unsigned char const* optr) {
    unsigned char const o = *optr;
    unsigned char const s = __sa_table[o >> 2];
    if (__sink_put(s)) {
      __to_second(o);
      return true;
    }
    return false;
  }
  
  bool __put_second(unsigned char const* optr) {
    unsigned char const o = *optr;
    unsigned char const s = __sa_table[((__last_o bitand 0x03) << 4) bitor (o >> 4)];
    if (__sink_put(s)) {
      __to_third(o);
      return true;
    }
    return false;
  }
  
  bool __put_third(unsigned char const* optr) {
    unsigned char const o = *optr;
    unsigned char const s1 = __sa_table[((__last_o bitand 0x0f) << 2) bitor (o >> 6)];
    unsigned char const s2 = __sa_table[o bitand 0x3f];
    if (__sink_put(s1) and __sink_put(s2)) {
      __to_first();
      return true;
    }
    return false;
  }
  
  void __conclude_first() {
    // Nothing to do
  }
  
  void __conclude_second() {
    unsigned char const s = __sa_table[(__last_o bitand 0x03) << 4];
    if (__sink_put(s) and __sink_put('=') and __sink_put('='))
      __to_first();
  }
  
  void __conclude_third() {
    unsigned char const s = __sa_table[(__last_o bitand 0x0f) << 2];
    if (__sink_put(s) and __sink_put('='))
      __to_first();
  }
  
  bool __sink_put(char c) {
    if (__line_size > 0 and ++__index == __line_size) {
      if (not Traits::put(__sink, '\n') or not Traits::put(__sink, '\r'))
        return false;
      __index = 0;
    }
    return Traits::put(__sink, c);
  }
};

} // end namespace ber

#endif // BERIO_B64_H
