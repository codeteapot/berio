#ifndef BERTREE_BERBOOLEAN_H
#define BERTREE_BERBOOLEAN_H

#include <istream>
#include <ostream>

#include <berio/types.h>

class ber_boolean {

  public:
  
  constexpr ber_boolean(bool value = false):
  __value(value) {}
  
  template<typename CharT, typename Traits>
  void do_extract(std::basic_istream<CharT, Traits>& is) {
    is >> std::boolalpha >> __value;
  }
  
  template<typename CharT, typename Traits>
  void do_insert(std::basic_ostream<CharT, Traits>& os) const {
    os << std::boolalpha << __value;
  }
  
  private:
  
  bool __value;
  
  template<typename>
  friend struct ber::boolean_traits;
};

template<>
struct ber::boolean_traits<ber_boolean> {

  static bool to_bool(ber_boolean const& b) { return b.__value; }
  
  static void assign(ber_boolean& b, bool bv) { b.__value = bv; }
};

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_boolean const& b) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (s)
    b.do_insert(os);
  return os;
}

#endif // BERTREE_BERBOOLEAN_H
