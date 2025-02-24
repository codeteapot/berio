#ifndef BERTREE_BERPRINTSTR_H
#define BERTREE_BERPRINTSTR_H

#include <algorithm>
#include <istream>
#include <ostream>
#include <vector>

#include <berio/types.h>

class ber_print_string {

  public:
  
  ber_print_string() {};
  
  private:
  
  std::vector<char> __value;
  
  template<typename>
  friend struct ber::print_string_traits;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>&,
      ber_print_string&);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>&,
      ber_print_string const&);
};

template<>
struct ber::print_string_traits<ber_print_string> {

  static char to_char(unsigned char o) { return static_cast<char>(o); }
  
  static void push_back(ber_print_string& pstr, char c) { pstr.__value.push_back(c); }
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_print_string& pstr) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_print_string const& pstr) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (s)
    std::for_each(pstr.__value.begin(), pstr.__value.end(), [&os](char const& c) { os << c; });
  return os;
}

#endif // BERTREE_BERPRINTSTR_H
