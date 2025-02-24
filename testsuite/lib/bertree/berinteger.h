#ifndef BERTREE_BERINTEGER_H
#define BERTREE_BERINTEGER_H

#include <istream>
#include <ostream>

#include <berio/types.h>

class ber_integer {

  public:
  
  constexpr ber_integer(int value = 0):
  __value(value) {}
  
  private:
  
  int __value;
  
  template<typename>
  friend struct ber::integer_traits;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>&,
      ber_integer&);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>&,
      ber_integer const&);
};

template<>
struct ber::integer_traits<ber_integer> {

  constexpr static size_t integer_size = sizeof (int);
  
  constexpr static ber_integer zero() { return 0; }
  
  static void l_shift(ber_integer& i, int n) { i.__value = i.__value << n; }
  
  static void bitor_assign(ber_integer& i, unsigned char o) { i.__value |= o; }
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_integer& i) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_integer const& i) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (s)
    os << std::dec << i.__value;
  return os;
}

#endif // BERTREE_BERINTEGER_H
