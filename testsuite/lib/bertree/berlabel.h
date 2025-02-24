#ifndef BERTREE_BERLABEL_H
#define BERTREE_BERLABEL_H

#include <istream>
#include <iterator>
#include <ostream>

#include <berio/tag.h>
#include <berio/univ.h>

#include "tokenmatch.h"

template<typename ValueT>
struct token_tuple {

  constexpr token_tuple(std::string_view t, ValueT v):
  token(t),
  value(v) {}
  
  std::string_view token;
  ValueT value;
};

class ber_label {

  public:
  
  ber_label(ber::tag_class = ber::tc_universal, ber::tag_number = 0, bool = false);
  
  ber::tag_class tclass() const;
  ber::tag_number number() const;
  bool definite() const;
  
  template<typename CharT, typename Traits>
  void do_extract(std::basic_istream<CharT, Traits>& is) {
    using char_t = typename Traits::char_type;
    
    std::locale const loc = is.getloc();
    
    auto known_univ_it = token_match<known_univ_tokens>(is);
    if (is.bad())
      return;
    if (not is) {
      is.clear();
      auto class_it = token_match<class_tokens>(is);
      if (not is)
        return;
      __tclass = class_it->value;
      is >> __number;
      if (is.bad())
        return;
      if (not is) {
        std::string_view const& token = class_it->token;
        for (std::size_t i = 0; i < token.size(); ++i)
          if (not is.putback(token[i])) {
            is.setstate(std::ios::badbit);
            return;
          }
      }
    }
    else {
      __tclass = ber::tc_universal;
      __number = known_univ_it->value;
    }
    
    __definite = true;
    char_t c;
    if (not is.get(c))
      return;
    if (is.narrow(c, '\0') not_eq ' ' or not is.get(c)) {
      if (not is.bad() and not is.putback(c))
        is.setstate(std::ios::badbit);
      return;
    }
    if (is.narrow(c, '\0') not_eq __indefinite_char or not is.get(c)) {
      if (not is.bad() and not (is.putback(c) and is.putback(' ')))
        is.setstate(std::ios::badbit);
      return;
    }
    if (not std::isspace(c, loc)) {
      if (not is.bad() and not (
          is.putback(c) and
          is.putback(__indefinite_char) and
          is.putback(' ')))
        is.setstate(std::ios::badbit);
      return;
    }
    if (not is.putback(c))
      is.setstate(std::ios::badbit);
    __definite = false;
  }
  
  template<typename CharT, typename Traits>
  void do_insert(std::basic_ostream<CharT, Traits>& os) const {
    if (__tclass == ber::tc_universal and known_univ_tokens::is_supported(__number))
      os << known_univ_tokens::token_of(__number);
    else
      os << class_tokens::token_of(__tclass) << ' ' << __number;
    if (not __definite)
      os << ' ' << ber_label::__indefinite_char;
  }
  
  private:
  
  // TODO tuples created created through constexpr function. Inverse map allowed
  
  struct class_tokens {

    static constexpr token_tuple<ber::tag_class> tuples[] = {
      {"UNIVERSAL", ber::tc_universal},
      {"APPLICATION", ber::tc_application},
      {"CONTEXT SPECIFIC", ber::tc_context_specific},
      {"PRIVATE", ber::tc_private}
    };
    
    static constexpr std::string_view const& token_of(ber::tag_class tclass) {
      return tuples[static_cast<std::size_t>(tclass)].token;
    }
  };
  
  struct known_univ_tokens {
  
    static constexpr token_tuple<ber::tag_number> tuples[] = {
      {"EOC", ber::univ::tn_eoc},
      {"BOOLEAN", ber::univ::tn_boolean},
      {"INTEGER", ber::univ::tn_integer},
      {"BIT STRING", ber::univ::tn_bit_string},
      {"OCTET STRING", ber::univ::tn_octet_string},
      {"NULL", ber::univ::tn_null},
      {"OBJECT IDENTIFIER", ber::univ::tn_object_ident},
      {"Object Descriptor", ber::univ::tn_object_descr},
      {"EXTERNAL", ber::univ::tn_external},
      {"REAL (float)", ber::univ::tn_real},
      {"ENUMERATED", ber::univ::tn_enumerated},
      {"EMBEDDED PDV", ber::univ::tn_emb_pdv},
      {"UTF8String", ber::univ::tn_utf8_string},
      {"RELATIVE-OID", ber::univ::tn_reloid},
      {"TIME", ber::univ::tn_time},
      {"UNIVERSAL 15", 15},
      {"SEQUENCE (OF)", ber::univ::tn_sequence},
      {"SET (OF)", ber::univ::tn_set},
      {"NumericString", ber::univ::tn_num_string},
      {"PrintableString", ber::univ::tn_print_string},
      {"T61String", ber::univ::tn_t61_string},
      {"VideotexString", ber::univ::tn_vtex_string},
      {"IA5String", ber::univ::tn_ia5_string},
      {"UTCTime", ber::univ::tn_utc_time},
      {"GeneralizedTime", ber::univ::tn_gen_time},
      {"GraphicString", ber::univ::tn_graph_string},
      {"VisibleString", ber::univ::tn_vis_string},
      {"GeneralString", ber::univ::tn_gen_string},
      {"UniversalString", ber::univ::tn_univ_string},
      {"CHARACTER STRING", ber::univ::tn_char_string},
      {"BMPString", ber::univ::tn_bmp_string},
      {"DATE", ber::univ::tn_date},
      {"TIME-OF-DAY", ber::univ::tn_timeofday},
      {"DATE-TIME", ber::univ::tn_datetime},
      {"DURATION", ber::univ::tn_duration},
      {"OID-IRI", ber::univ::tn_oidiri},
      {"RELATIVE-OID-IRI",  ber::univ::tn_reloidiri}
    };
    
    static constexpr bool is_supported(ber::tag_number number) {
      return number < std::size(tuples);
    }
    
    static constexpr std::string_view const& token_of(ber::tag_number number) {
      return tuples[static_cast<std::size_t>(number)].token;
    }
  };
  
  constexpr static char __indefinite_char = '*';
  
  ber::tag_class __tclass;
  ber::tag_number __number;
  bool __definite;
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_label& label) {
  typename std::basic_istream<CharT, Traits>::sentry s(is);
  if (not s)
    return is;
  label.do_extract(is);
  return is;
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (
    std::basic_ostream<CharT, Traits>& os,
    ber_label const& label) {
  typename std::basic_ostream<CharT, Traits>::sentry s(os);
  if (not s)
    return os;
  label.do_insert(os);
  return os;
}

#endif // BERTREE_BERLABEL_H
