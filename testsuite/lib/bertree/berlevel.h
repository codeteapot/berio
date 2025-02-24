#ifndef BERTREE_BERLEVEL_H
#define BERTREE_BERLEVEL_H

#include <istream>
#include <ostream>

class ber_level {

  public:
  
  ber_level(unsigned int = 0);
  
  bool operator == (ber_level const&) const;
  ber_level operator + (unsigned int amount) const;
  
  template<typename CharT, typename Traits>
  void do_extract(std::basic_istream<CharT, Traits>& is) {
    using char_t = typename std::basic_istream<CharT, Traits>::char_type;
    
    char_t const w_first_char = is.widen(__first_char);
    char_t const w_amount_char = is.widen(__amount_char);
    char_t c;
    
    if (not is.get(c))
      return;
    if (not Traits::eq(c, w_first_char)) {
      if (not is.putback(c)) {
        is.setstate(std::ios::badbit);
        return;
      }
      is.setstate(std::ios::failbit);
      return;
    }
    
    std::locale loc = is.getloc();
    unsigned int amount = 0;
    
    if (not is.get(c))
      return;
    while (not std::isspace(c, loc)) {
      if (not Traits::eq(c, w_amount_char)) {
        for (unsigned int i = 0; i < amount; ++i)
          if (not is.putback(w_amount_char)) {
            is.setstate(std::ios::badbit);
            return;
          }
        if (not is.putback(c)) {
          is.setstate(std::ios::badbit);
          return;
        }
        is.setstate(std::ios::failbit);
        return;
      }
      ++amount;
      if (not is.get(c)) {
        for (unsigned int i = 0; i < amount; ++i)
          if (not is.putback(w_amount_char)) {
            is.setstate(std::ios::badbit);
            break;
          }
        return;
      }
    }
    is.putback(c);
    __amount = amount;
  }
  
  template<typename CharT, typename Traits>
  void do_insert(std::basic_ostream<CharT, Traits>& os) const {
    using char_t = typename std::basic_istream<CharT, Traits>::char_type;
    
    char_t const w_first_char = os.widen(__first_char);
    char_t const w_amount_char = os.widen(__amount_char);
    
    os.put(w_first_char); 
    std::fill_n(std::ostream_iterator<CharT>(os), __amount, w_amount_char);
  }
  
  private:
  
  constexpr static char __first_char = '+';
  constexpr static char __amount_char = '-';
  
  unsigned __amount;
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_level& level) {
  typename std::basic_istream<CharT, Traits>::sentry s(is);
  if (not s)
    return is;
  level.do_extract(is);
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_level const& level) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (not s)
    return os;
  level.do_insert(os);
  return os;
}

#endif // BERTREE_BERLEVEL_H
