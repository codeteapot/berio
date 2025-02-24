#ifndef DERTREE_BERTREE_H
#define DERTREE_BERTREE_H

#include <istream>
#include <iterator>
#include <ostream>
#include <vector>

#include <berio/tag.h>

class ber_level {

  public:
  
  ber_level(unsigned);
  
  ber_level operator + (unsigned amount) const;
  
  private:
  
  constexpr static char __first_char = '+';
  constexpr static char __amount_char = '-';
  
  unsigned __amount;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>& is,
      ber_level& level);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>& os,
      ber_level const& level);
};

class ber_label {

  public:
  
  ber_label(ber::tag_class const&, ber::tag_number const&, bool);
  
  ber::tag_class const& tclass() const;
  ber::tag_number const& number() const;
  bool definite() const;
  
  private:
  
  constexpr static char const* __class_univ = "UNIVERSAL";
  constexpr static char const* __class_app = "APPLICATION";
  constexpr static char const* __class_ctx_spec = "CONTEXT SPECIFIC";
  constexpr static char const* __class_priv = "PRIVATE";
  
  constexpr static char const* __univ_class[] {
    "EOC",
    "BOOLEAN",
    "INTEGER",
    "BIT STRING",
    "OCTET STRING",
    "NULL",
    "OBJECT IDENTIFIER",
    "Object Descriptor",
    "EXTERNAL",
    "REAL (float)",
    "ENUMERATED",
    "EMBEDDED PDV",
    "UTF8String",
    "RELATIVE-OID",
    "TIME",
    "UNIVERSAL 15",
    "SEQUENCE (OF)",
    "SET (OF)",
    "NumericString",
    "PrintableString",
    "T61String",
    "VideotexString",
    "IA5String",
    "UTCTime",
    "GeneralizedTime",
    "GraphicString",
    "VisibleString",
    "GeneralString",
    "UniversalString",
    "CHARACTER STRING",
    "BMPString",
    "DATE",
    "TIME-OF-DAY",
    "DATE-TIME",
    "DURATION",
    "OID-IRI",
    "RELATIVE-OID-IRI"
  };
  
  constexpr static char __indefinite_char = '*';
  
  ber::tag_class __tclass;
  ber::tag_number __number;
  bool __definite;
  
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>& is,
      ber_label& label);
  
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>& os,
      ber_label const& label);
};

class ber_content {
  
  public:
  
  ber_content() = delete;
  
  template<typename CharT, typename Traits>
  static std::basic_ostream<CharT, Traits>& begin(std::basic_ostream<CharT, Traits>& os) {
    return os << __begin_char;
  }
  
  template<typename CharT, typename Traits>
  static std::basic_ostream<CharT, Traits>& end(std::basic_ostream<CharT, Traits>& os) {
    return os << __end_char;
  }
  
  private:
  
  constexpr static char __begin_char = '<';
  constexpr static char __end_char = '>';
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_level& level) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_level const& level) {
  typename std::basic_ostream<CharT, Traits>::sentry const sty(os);
  if (sty) {
    os.put(ber_level::__first_char);
    std::fill_n(std::ostream_iterator<char>(os), level.__amount, ber_level::__amount_char);
  }
  return os;
}

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_label& label) {
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_label const& label) {
  typename std::basic_ostream<CharT, Traits>::sentry const sty(os);
  if (sty) {
    switch (label.__tclass) {
      case ber::tc_universal:
        if (label.__number < sizeof (ber_label::__univ_class))
          os << ber_label::__univ_class[label.__number];
        else
          os << ber_label::__class_univ << ' ' << label.__number;
        break;
      case ber::tc_application:
        os << ber_label::__class_app << ' ' << label.__number;
        break;
      case ber::tc_context_specific:
        os << ber_label::__class_ctx_spec << ' ' << label.__number;
        break;
      case ber::tc_private:
        os << ber_label::__class_priv << ' ' << label.__number;
        break;
    }
    if (not label.__definite)
      os << ' ' << ber_label::__indefinite_char;
  }
  return os;
}

#endif // DERTREE_BERTREE_H
