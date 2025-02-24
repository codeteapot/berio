#ifndef BERTREE_BERCONTENT_H
#define BERTREE_BERCONTENT_H

#include <istream>
#include <ostream>

template<typename CharT, typename Traits>
class ber_basic_contentbuf : public std::basic_stringbuf<CharT, Traits> {

  public:
  
  ber_basic_contentbuf(std::basic_streambuf<CharT, Traits>* prev):
  __prev(prev) {}
  
  std::basic_streambuf<CharT, Traits>* rdprev() const {
    return __prev;
  }
  
  private:
  
  std::basic_streambuf<CharT, Traits>* __prev;
};

class ber_content {
  
  public:
  
  ber_content() = delete;
  
  template<typename CharT, typename Traits>
  static std::basic_istream<CharT, Traits>& begin(std::basic_istream<CharT, Traits>& is) {
    using ber_contentbuf = ber_basic_contentbuf<CharT, Traits>;
    using char_t = typename Traits::char_type;
    using int_t = typename Traits::int_type;
    if (not is)
      return is;
    char_t c;
    if (not is.get(c))
      return is;
    if (__begin_char not_eq is.narrow(c, '\0')) {
      if (not is.putback(c)) {
        is.setstate(std::ios::badbit);
        return is;
      }
      is.setstate(std::ios::failbit);
      return is;
    }
    ber_contentbuf* buf = new ber_contentbuf(is.rdbuf());
    if (not is.get(c)) {
      if (not is.putback(is.widen(__begin_char)))
        is.setstate(std::ios::badbit);
      delete buf;
      return is;
    }
    while (is.narrow(c, '\0') not_eq __end_char)
      if (buf->sputc(c) == Traits::eof() or not is.get(c)) {
        int_t prev_ic = buf->sungetc();
        while (prev_ic not_eq Traits::eof())
          if (not is.putback(Traits::to_char_type(prev_ic))) {
            is.setstate(std::ios::badbit);
            delete buf;
            return is;
          }
        if (not is.putback(is.widen(__begin_char))) {
          is.setstate(std::ios::badbit);
          delete buf;
          return is;
        }
        is.setstate(std::ios::failbit);
        delete buf;
        return is;
      }
    is.rdbuf(buf);
    return is;
  }
  
  template<typename CharT, typename Traits>
  static std::basic_ostream<CharT, Traits>& begin(std::basic_ostream<CharT, Traits>& os) {
    if (not os)
      return os;
    return os << os.widen(__begin_char);
  }
  
  template<typename CharT, typename Traits>
  static std::basic_istream<CharT, Traits>& end(std::basic_istream<CharT, Traits>& is) {
    using ber_contentbuf = ber_basic_contentbuf<CharT, Traits>;
    using char_t = typename Traits::char_type;
    if (not is)
      return is;
    if (ber_contentbuf* buf = dynamic_cast<ber_contentbuf*>(is.rdbuf()); buf not_eq nullptr) {
      is.rdbuf(buf->rdprev());
      delete buf;
      return is;
    }
    is.setstate(std::ios::failbit);
    return is;
  }
  
  template<typename CharT, typename Traits>
  static std::basic_ostream<CharT, Traits>& end(std::basic_ostream<CharT, Traits>& os) {
    if (not os)
      return os;
    return os << os.widen(__end_char);
  }
  
  private:
  
  static constexpr char __begin_char = '<';
  static constexpr char __end_char = '>';
};

#endif // BERTREE_BERCONTENT_H
