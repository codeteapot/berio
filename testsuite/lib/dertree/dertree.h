#ifndef DERTREE_DERTREE_H
#define DERTREE_DERTREE_H

#include <istream>
#include <iterator>
#include <ostream>

#include <berio/encoding.h>
#include <berio/tag.h>

class der_tree {

  public:
  
  explicit der_tree(int = 0);
  
  bool empty() const;
  bool end() const;
  bool error() const;
  
  ber::tag_ident const& ident() const;
  ber::tag_length const& length() const;
  void length(ber::tag_length const&);
  
  void encode(ber::octet_output&) const;
  void decode(ber::octet_input&);
  
  private:
  
  constexpr static int __margin_size = 2;
  
  constexpr static char __error_prefix = '!';
  
  constexpr static char const* __error_incomplete = "incomplete";
  constexpr static char const* __error_overflow = "overflow";
  constexpr static char const* __error_invalid = "invalid";
  constexpr static char const* __error_indef_prim = "indefinite-primitive";
  
  constexpr static char const* __class_label_univ = "UNIVERSAL";
  constexpr static char const* __class_label_app = "APPLICATION";
  constexpr static char const* __class_label_ctx_spec = "CONTEXT SPECIFIC";
  constexpr static char const* __class_label_priv = "PRIVATE";
  
  constexpr static char const* __univ_class_labels[] {
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
  
  int __level;
  ber::tag_decode_result __tdr;
  ber::tag_header __header;
  
  bool __indefinite_primitive() const;
  
  template<typename charT, typename Traits>
  friend std::basic_istream<charT, Traits>& operator >> (
      std::basic_istream<charT, Traits>& is,
      der_tree& tree) {
    if (not is.good())
      return is;
    typename std::basic_istream<charT, Traits>::sentry sty(is);
    if (sty) {
      // TODO ...
    }
    return is;
  }
  
  template<typename charT, typename Traits>
  friend std::basic_ostream<charT, Traits>& operator << (
      std::basic_ostream<charT, Traits>& os,
      der_tree const& tree) {
    if (not os.good())
      return os;
    typename std::basic_ostream<charT, Traits>::sentry const sty(os);
    if (sty) {
      switch (tree.__tdr) {
        case ber::tdr_incomplete:
          os << __error_prefix << __error_incomplete << std::endl;
          break;
        case ber::tdr_overflow:
          os << __error_prefix << __error_overflow << std::endl;
          break;
        case ber::tdr_invalid:
          os << __error_prefix << __error_invalid << std::endl;
          break;
        case ber::tdr_complete:
          if (tree.__indefinite_primitive()) {
            os << __error_prefix << __error_indef_prim << std::endl;
            return os;
          }
          std::fill_n(std::ostream_iterator<char>(os), tree.__level * __margin_size, ' ');
          switch (tree.__header.ident.tclass) {
            case ber::tc_universal:
              if (tree.__header.ident.tnumber < sizeof (__univ_class_labels))
                os << __univ_class_labels[tree.__header.ident.tnumber];
              else
                os << __class_label_univ << ' ' << tree.__header.ident.tnumber;
              break;
            case ber::tc_application:
              os << __class_label_app << ' ' << tree.__header.ident.tnumber;
              break;
            case ber::tc_context_specific:
              os << __class_label_ctx_spec << ' ' << tree.__header.ident.tnumber;
              break;
            case ber::tc_private:
              os << __class_label_priv << ' ' << tree.__header.ident.tnumber;
              break;
          }
          break;
      }
    }
    return os;
  }
};

#endif // DERTREE_DERTREE_H
