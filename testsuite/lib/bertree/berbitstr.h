#ifndef BERTREE_BERBITSTR_H
#define BERTREE_BERBITSTR_H

#include <algorithm>
#include <iomanip>
#include <istream>
#include <ostream>
#include <vector>

#include <berio/univ.h>

class ber_bit_string {

  public:
  
  ber_bit_string() {};
  
  private:
  
  std::vector<bool> __value;
  
  template<typename>
  friend struct ber::univ::bit_string_traits;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>&,
      ber_bit_string&);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>&,
      ber_bit_string const&);
};

template<>
struct ber::univ::bit_string_traits<ber_bit_string> {

  static void push_back(ber_bit_string& bstr, bool b) { bstr.__value.push_back(b); }
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_bit_string& bstr) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_bit_string const& bstr) {
  typename std::basic_ostream<CharT, Traits>::sentry const sty(os);
  if (sty) {
    os << std::noboolalpha;
    std::string sep = "";
    std::for_each(bstr.__value.begin(), bstr.__value.end(), [&os, &sep](bool const& b) {
      os << sep << b;
      sep = " ";
    });
    os << std::resetiosflags(std::ios_base::basefield);
  }
  return os;
}

#endif // BERTREE_BERBITSTR_H
