#include <iomanip>
#include <ostream>

#include <berio/deliminput.h>
#include <berio/encoding.h>
#include <berio/univ.h>

#include "berformat.h"
#include "bertree.h"

using namespace std;
using namespace ber;
using namespace ber::univ;

void tree_content_boolean(octet_input& in, ostream& os) {
  if (not tag_decode_boolean(in, [&os](bool b) { os << (b ? "true" : "false"); }))
    os.setstate(os.rdstate() bitor ios::badbit);
}

void tree_content_integer(octet_input& in, ostream& os) {
  if (not tag_decode_integer<int>(in, [&os](int i) { os << i; }))
    os.setstate(os.rdstate() bitor ios::badbit);
}

void tree_content_bit_string(octet_input& in, ostream& os) {
  string sep = "";
  if (not tag_decode_bit_string<int>(in, [&os, &sep](int b) {
    os << sep << b;
    sep = " ";
  }))
    os.setstate(os.rdstate() bitor ios::badbit);
}

void tree_content_octet_string(octet_input& in, ostream& os) {
  ios::fmtflags f = os.flags();
  os.flags(ios::hex);
  string sep = "";
  if (not tag_decode_octet_string<int>(in, [&os, &sep](int i) {
    os << sep << setfill('0') << setw(2) << i;
    sep = " ";
  }))
    os.setstate(os.rdstate() bitor ios::badbit);
  os.flags(f);
}

void tree_content_utf8_string(octet_input& in, ostream& os) {
  if (not tag_decode_utf8_string(in, [&os](char32_t c) { os << c; }))
    os.setstate(os.rdstate() bitor ios::badbit);
}

void tree_content_print_string(octet_input& in, ostream& os) {
  if (not tag_decode_print_string<char>(in, [&os](char c) { os << c; }))
    os.setstate(os.rdstate() bitor ios::badbit);
}

void tree_content_oid(octet_input& in, ostream& os) {
  string sep = "";
  if (not tag_decode_oid<unsigned>(in, [&os, &sep](unsigned i) {
    os << sep << i;
    sep = ".";
  }))
    os.setstate(os.rdstate() bitor ios::badbit);
}

void tree_content_utc_time(octet_input& in, ostream& os) {
  // return get_utc_time(in, [&os](const iso8601_time_t& t) {});
  if (not tag_decode_print_string<char>(in, [&os](char c) { os << c; }))
    os.setstate(os.rdstate() bitor ios::badbit);
}

typedef void (* tree_content_func)(octet_input&, ostream&);

constexpr tree_content_func univ_tree_content_fn[] = {
  &tree_content_octet_string, // EOC
  &tree_content_boolean,
  &tree_content_integer,
  &tree_content_bit_string,
  &tree_content_octet_string, // OCTET STRING
  &tree_content_octet_string, // NULL
  &tree_content_oid,
  &tree_content_octet_string, // Object Descriptor
  &tree_content_octet_string, // EXTERNAL
  &tree_content_octet_string, // REAL (float)
  &tree_content_octet_string, // ENUMERATED
  &tree_content_octet_string, // EMBEDDED PDV
  &tree_content_utf8_string,
  &tree_content_octet_string, // RELATIVE-OID
  &tree_content_octet_string, // TIME
  &tree_content_octet_string, // UNIVERSAL 15
  &tree_content_octet_string, // SEQUENCE (OF)
  &tree_content_octet_string, // SET (OF)
  &tree_content_octet_string, // NumericString
  &tree_content_print_string,
  &tree_content_octet_string, // T61String
  &tree_content_octet_string, // VideotexString
  &tree_content_octet_string, // IA5String
  &tree_content_utc_time,
  &tree_content_octet_string, // GeneralizedTime
  &tree_content_octet_string, // GraphicString
  &tree_content_octet_string, // VisibleString
  &tree_content_octet_string, // GeneralString
  &tree_content_octet_string, // UniversalString
  &tree_content_octet_string, // CHARACTER STRING
  &tree_content_octet_string, // BMPString
  &tree_content_octet_string, // DATE
  &tree_content_octet_string, // TIME-OF-DAY
  &tree_content_octet_string, // DATE-TIME
  &tree_content_octet_string, // DURATION
  &tree_content_octet_string, // OID-IRI
  &tree_content_octet_string, // RELATIVE-OID-IRI
};

constexpr tree_content_func tree_content(tag_ident const& ident) {
  return ident.tclass == tc_universal and ident.tnumber < sizeof (univ_tree_content_fn)
    ? univ_tree_content_fn[ident.tnumber]
    : tree_content_octet_string;
}

bool tree_scan(delimited_input& in, ostream& os, tree_level level) {
  tag_header header;
  // TODO Wrap input in order to have failure trace
  switch (tag_decode(header, in)) {
    case tdr_empty:
      return false;
    case tdr_incomplete:
      os << "! hex-trace (incomplete)" << endl;
      return false;
    case tdr_overflow:
      os << "! hex-trace (overflow)" << endl;
      return false;
    case tdr_invalid:
      os << "! hex-trace (invalid)" << endl;
      return false;
  }
  tree_label label;
  label.tclass = header.ident.tclass;
  label.tnumber = header.ident.tnumber;
  tree_definite defin = header.length.definite;
  os << level << label << defin;
  delimited_input::delimit_sentry const delimit_sentry(in, header.length);
  if (header.ident.constructed) {
    os << endl;
    while (tree_scan(in, os, ++level));
    return os.good();
  }
  os << " [";
  tree_content(header.ident)(in, os);
  os << "]" << endl;
  return os.good() and header.ident;
}

void tree_scan(octet_input& in, ostream& os) {
  delimited_input delim_in(in);
  while (tree_scan(delim_in, os, 0));
}
