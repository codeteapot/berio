#ifndef BERTREE_BERBOOLEAN_H
#define BERTREE_BERBOOLEAN_H

#include <istream>
#include <ostream>

#include <berio/univ.h>

class ber_boolean {

  public:
  
  constexpr ber_boolean(bool value = false):
  __value(value) {}
  
  private:
  
  bool __value;
  
  template<typename>
  friend struct ber::univ::boolean_traits;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>&,
      ber_boolean&);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>&,
      ber_boolean const&);
};

template<>
struct ber::univ::boolean_traits<ber_boolean> {

  static void assign(ber_boolean& b, bool bv) { b.__value = bv; }
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_boolean& b) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_boolean const& b) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (s)
    os << std::boolalpha << b.__value;
  return os;
}

#endif // BERTREE_BERBOOLEAN_H
