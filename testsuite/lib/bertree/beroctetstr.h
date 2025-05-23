#ifndef BERTREE_BEROCTETSTR_H
#define BERTREE_BEROCTETSTR_H

#include <algorithm>
#include <iomanip>
#include <istream>
#include <ostream>
#include <vector>

#include <berio/univ.h>

class ber_octet_string {

  public:
  
  ber_octet_string() {};
  
  private:
  
  std::vector<unsigned char> __value;
  
  template<typename>
  friend struct ber::univ::octet_string_traits;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>&,
      ber_octet_string&);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>&,
      ber_octet_string const&);
};

template<>
struct ber::univ::octet_string_traits<ber_octet_string> {

  static void push_back(ber_octet_string& ostr, unsigned char o) { ostr.__value.push_back(o); }
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_octet_string& ostr) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_octet_string const& ostr) {
  typename std::basic_ostream<CharT, Traits>::sentry const sty(os);
  if (sty) {
    std::string sep = "";
    os << std::hex;
    std::for_each(ostr.__value.begin(), ostr.__value.end(), [&os, &sep](unsigned char const& o) {
      os << sep << std::setfill('0') << std::setw(2) << static_cast<int>(o);
      sep = " ";
    });
    os << std::resetiosflags(std::ios_base::basefield);
  }
  return os;
}

#endif // BERTREE_BEROCTETSTR_H
