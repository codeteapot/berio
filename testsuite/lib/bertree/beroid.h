#ifndef BERTREE_BEROID_H
#define BERTREE_BEROID_H

#include <algorithm>
#include <istream>
#include <ostream>
#include <vector>

#include <berio/types.h>

class ber_oid {

  public:
  
  ber_oid() {};
  
  private:
  
  std::vector<int> __value;
  
  template<typename>
  friend struct ber::oid_traits;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>&,
      ber_oid&);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>&,
      ber_oid const&);
};

template<>
struct ber::oid_traits<ber_oid> {

  typedef unsigned long comp_type;
  
  constexpr static size_t comp_size = sizeof (unsigned long);
  
  constexpr static unsigned long comp_zero() { return 0; }
  
  static unsigned long int_to_comp(int i) { return static_cast<unsigned long>(i); }
  
  static bool comp_lt(unsigned long comp1, unsigned long comp2) { return comp1 < comp2; }
  
  static unsigned long comp_minus(unsigned long comp1, unsigned long comp2) {
    return comp1 - comp2;
  }
  
  static void comp_l_shift(unsigned long& comp, int n) { comp = comp << n; }
  
  static void comp_bitor_assign(unsigned long& comp, unsigned char o) { comp |= o; }
  
  static void push_back(ber_oid& oid, unsigned long comp) { oid.__value.push_back(comp); }
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_oid& oid) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_oid const& oid) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (s) {
    std::string sep = "";
    std::for_each(oid.__value.begin(), oid.__value.end(), [&os, &sep](long const& i) {
      os << sep << i;
      sep = ".";
    });
  }
  return os;
}

#endif // BERTREE_BEROID_H
