#ifndef DERTREE_DERTREE_H
#define DERTREE_DERTREE_H

#include <istream>
#include <iterator>
#include <ostream>
// TODO Remove file

#include <berio/encoding.h>
#include <berio/tag.h>

class der_tree {

  public:
  
  explicit der_tree(int = 0);
  
  bool empty() const;
  bool complete() const;
  bool end() const;
  bool constructed() const;
  bool definite() const;
  
  std::size_t length() const;
  void length(std::size_t);
  
  void print(ber::octet_output&) const;
  void scan(ber::octet_input&);
  
  private:
  
  constexpr static int __margin_size = 2;
  
  constexpr static char __error_prefix = '!';
  
  constexpr static char const* __error_indefinite_primitive = "indefinite-primitive";
  constexpr static char const* __error_number_incomplete = "number-incomplete";
  constexpr static char const* __error_number_overflow = "number-overflow";
  constexpr static char const* __error_length_undefined = "length-undefined";
  constexpr static char const* __error_length_unsupported = "length-unsupported";
  constexpr static char const* __error_length_incomplete = "length-incomplete";
  constexpr static char const* __error_length_overflow = "length_overflow";
  
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
  ber::tag_header __th;
  
  // https://en.cppreference.com/w/cpp/string/basic_string/operator_cmp (8)
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>& operator >> (
      std::basic_istream<CharT, Traits>& is,
      der_tree& tree) {
    typedef typename std::basic_istream<CharT, Traits>::int_type int_type_t;
    if (not is.good())
      return is;
    typename std::basic_istream<CharT, Traits>::sentry sty(is);
    if (sty) {
      int_type_t c = is.peek();
      if (not is.good())
        return is.setstate(std::ios::failbit), is;
      if (Traits::to_char_type(c) == is.widen(__error_prefix)) {
        is.ignore();
        std::basic_string<CharT, Traits> error;
        is >> std::noskipws >> error;
        if (not is.good())
          return is;
        if (error == __error_indefinite_primitive)
          tree.__tdr = ber::tdr_indefinite_primitive;
        else if (error == __error_number_incomplete)
          tree.__tdr = ber::tdr_number_incomplete;
        else if (error == __error_number_overflow)
          tree.__tdr = ber::tdr_number_overflow;
        else if (error == __error_length_undefined)
          tree.__tdr = ber::tdr_length_undefined;
        else if (error == __error_length_unsupported)
          tree.__tdr = ber::tdr_length_unsupported;
        else if (error == __error_length_incomplete)
          tree.__tdr = ber::tdr_length_incomplete;
        else if (error == __error_length_overflow)
          tree.__tdr = ber::tdr_length_overflow;
        else
          return is.setstate(std::ios::badbit), is;
        // TODO Read hex trace
        tree.__level = 0;
        tree.__th = { { ber::tc_universal, false, 0 }, { 0, false } };
      }
      else {
        int level = 0;
        int marg = 0;
        while (Traits::to_char_type(c) == is.widen(' ')) {
	  is.ignore();
	  ++marg;
	  c = is.peek();
	  if (not is.good())
	    return is;
	  if (Traits::to_char_type(c) == is.widen('\n'))
	    return is.setstate(std::ios::badbit), is;
        }
        if (marg % __margin_size == 0)
	  level = marg / __margin_size;
        else
	  return is.setstate(std::ios::badbit), is;
	std::basic_ostringstream<CharT, Traits> label_os;
	bool label_space = false;
	bool label_end = false;
	bool constructed = false;
	do {
	  c = is.get();
	  if (not is.good())
	    return is;
	  if (label_space) {
	    if (Traits::to_char_type(c) == is.widen('\n'))
	      return is.setstate(std::ios::badbit), is;
	    if (Traits::to_char_type(c) == is.widen(der_tree::primitive_prefix)) {
	      label_end = true;
	      constructed = false; // Initial value
	    }
	    else if (
	        std::isalnum(is.narrow(Traits::to_char_type(c), '?')) or
	        Traits::to_char_type(c) ==  is.widen('(') or
	        Traits::to_char_type(c) ==  is.widen(')') or
	        Traits::to_char_type(c) ==  is.widen('-')) {
	      label_space = false;
	      label_os.put(is.widen(' '));
	      label_os.put(Traits::to_char_type(c));
	    }
	    else
	      return is.setstate(std::ios::badbit), is;
	  }
	  else {
	    if (Traits::to_char_type(c) == is.widen(der_tree::primitive_prefix))
	      return is.setstate(std::ios::badbit), is;
	    if (Traits::to_char_type(c) == is.widen('\n')) {
	      label_end = true;
	      constructed = true;
	    }
	    else if (Traits::to_char_type(c) == is.widen(' '))
	      label_space = true;
	    else if (
	        std::isalnum(is.narrow(Traits::to_char_type(c), '?')) or
	        Traits::to_char_type(c) ==  is.widen('(') or
	        Traits::to_char_type(c) ==  is.widen(')') or
	        Traits::to_char_type(c) ==  is.widen('-'))
	      label_os.put(Traits::to_char_type(c));
	    else
	      return is.setstate(std::ios::badbit), is;
	  }
	}
	while (not label_end);
	std::basic_string<CharT, Traits> label = label_os.str();
	ber::tag_class tclass = ber::tc_universal;
	ber::tag_number tnumber = 0;
	bool univ_found = false;
	std::size_t univ_i = 0;
	while (not univ_found and univ_i < std::size(__univ_class_labels))
	  if (not (univ_found = label == __univ_class_labels[univ_i]))
	    ++univ_i, ++tnumber;
	if (not univ_found) {
	  std::basic_ostringstream<CharT, Traits> label_nu_os;
	  std::basic_istringstream<CharT, Traits> label_is(label);
	  std::string part_str;
	  label_is >> part_str;
	  while (not label_is.eof()) {
	    label_nu_os << part_str;
	    label_is >> tnumber;
	    if (label_is.rdstate() bitand std::ios::failbit) {
	      label_nu_os << ' ';
	      label_is.clear();
	      label_is >> part_str;
	    }
	  }
	  std::string nu_label = label_nu_os.str();
	  if (nu_label == __class_label_univ)
	    tclass = ber::tc_universal; // Initial value
	  else if (nu_label == __class_label_app)
	    tclass = ber::tc_application;
	  else if (nu_label == __class_label_ctx_spec)
	    tclass = ber::tc_context_specific;
	  else if (nu_label == __class_label_priv)
	    tclass = ber::tc_private;
	  else
	    return is.setstate(std::ios::badbit), is;
	}
	tree.__level = level;
	tree.__th = { { tclass, constructed, tnumber }, { 0, true } };
	tree.__tdr = ber::tdr_complete;
      }
    }
    return is;
  }
  
  // https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt2 (2)
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>& operator << (
      std::basic_ostream<CharT, Traits>& os,
      der_tree const& tree) {
    if (not os.good())
      return os;
    typename std::basic_ostream<CharT, Traits>::sentry const sty(os);
    if (sty) {
      switch (tree.__tdr) {
        case ber::tdr_indefinite_primitive:
          os << __error_prefix << __error_indefinite_primitive;
          // TODO Write hex trace
          break;
        case ber::tdr_number_incomplete:
          os << __error_prefix << __error_number_incomplete;
          // TODO Write hex trace
          break;
        case ber::tdr_number_overflow:
          os << __error_prefix << __error_number_overflow;
          // TODO Write hex trace
          break;
        case ber::tdr_length_undefined:
          os << __error_prefix << __error_length_undefined;
          // TODO Write hex trace
          break;
        case ber::tdr_length_unsupported:
          os << __error_prefix << __error_length_unsupported;
          // TODO Write hex trace
          break;
        case ber::tdr_length_incomplete:
          os << __error_prefix << __error_length_incomplete;
          // TODO Write hex trace
          break;
        case ber::tdr_length_overflow:
          os << __error_prefix << __error_length_overflow;
          // TODO Write hex trace
          break;
        case ber::tdr_complete:
          std::fill_n(std::ostream_iterator<char>(os), tree.__level * __margin_size, ' ');
          switch (tree.__th.tclass) {
            case ber::tc_universal:
              if (tree.__th.number < sizeof (__univ_class_labels))
                os << __univ_class_labels[tree.__th.number];
              else
                os << __class_label_univ << ' ' << tree.__th.number;
              break;
            case ber::tc_application:
              os << __class_label_app << ' ' << tree.__th.number;
              break;
            case ber::tc_context_specific:
              os << __class_label_ctx_spec << ' ' << tree.__th.number;
              break;
            case ber::tc_private:
              os << __class_label_priv << ' ' << tree.__th.number;
              break;
          }
          break;
      }
    }
    return os;
  }
};

#endif // DERTREE_DERTREE_H
