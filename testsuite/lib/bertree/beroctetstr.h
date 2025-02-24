#ifndef BERTREE_BEROCTETSTR_H
#define BERTREE_BEROCTETSTR_H

#include <algorithm>
#include <iomanip>
#include <istream>
#include <iterator>
#include <ostream>
#include <vector>

#include <berio/types.h>

class ber_octet_string {

  public:
  
  ber_octet_string() {};
  
  ber_octet_string(ber_octet_string const&) = delete;
  
  template<typename CharT, typename Traits>
  void do_extract(std::basic_istream<CharT, Traits>& is) {
    is >> std::hex;
    std::copy(
      std::istream_iterator<unsigned int>(is),
      std::istream_iterator<unsigned int>(),
      std::back_inserter(__value));
    is >> std::resetiosflags(std::ios_base::basefield);
  }
  
  template<typename CharT, typename Traits>
  void do_insert(std::basic_ostream<CharT, Traits>& os) const {
    std::string sep = "";
    os << std::hex;
    std::for_each(__value.begin(), __value.end(), [&os, &sep](unsigned char const& o) {
      os << sep << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(o);
      sep = " ";
    });
    os << std::resetiosflags(std::ios_base::basefield);
  }
  
  private:
  
  std::vector<unsigned char> __value;
  
  template<typename>
  friend struct ber::octet_string_traits;
};

template<>
struct ber::octet_string_traits<ber_octet_string> {

  using container_type = std::vector<unsigned char>;
  using size_type = typename container_type::size_type;
  using value_type = typename container_type::value_type;
  using const_iterator = typename container_type::const_iterator;
  
  static std::size_t to_size_t(size_type s) { return s; }
  
  static unsigned char to_octet(value_type v) { return v; }
  
  static size_type size(ber_octet_string const& ostr) { return ostr.__value.size(); }
  
  static const_iterator begin(ber_octet_string const& ostr) { return std::begin(ostr.__value); }
  
  static const_iterator end(ber_octet_string const& ostr) { return std::end(ostr.__value); }
  
  static void push_back(ber_octet_string& ostr, unsigned char o) { ostr.__value.push_back(o); }
};

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_octet_string const& ostr) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (s)
    ostr.do_insert(os);
  return os;
}

#endif // BERTREE_BEROCTETSTR_H
