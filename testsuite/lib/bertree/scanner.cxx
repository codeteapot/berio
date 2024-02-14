#include <iomanip>
#include <iterator>
#include <ostream>

#include <berio/univ.h>

#include "bertree.h"

using namespace std;
using namespace ber;
using namespace ber::univ;

constexpr size_t known_tag_count = 37;

char const* const known_tag_label[] {
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

bool tag_content_boolean(octet_input& in, ostream& os) {
  return tag_decode_boolean(in, [&os](bool b) { os << (b ? "true" : "false"); });
}

bool tag_content_integer(octet_input& in, ostream& os) {
  return tag_decode_integer<int>(in, [&os](int i) { os << i; });
}

bool tag_content_bit_string(octet_input& in, ostream& os) {
  string sep = "";
  bool result = tag_decode_bit_string<int>(in, [&os, &sep](int b) {
    os << sep << b;
    sep = " ";
  });
  return result;
}

bool tag_content_octet_string(octet_input& in, ostream& os) {
  std::ios::fmtflags f = os.flags();
  os.flags(std::ios::hex);
  string sep = "";
  bool result = tag_decode_octet_string<int>(in, [&os, &sep](int i) {
    os << sep << setfill('0') << setw(2) << i;
    sep = " ";
  });
  os.flags(f);
  return result;
}

bool tag_content_utf8_string(octet_input& in, ostream& os) {
  return tag_decode_utf8_string(in, [&os](char32_t c) { os << c; });
}

bool tag_content_print_string(octet_input& in, ostream& os) {
  return tag_decode_print_string<char>(in, [&os](char c) { os << c; });
}

bool tag_content_oid(octet_input& in, ostream& os) {
  bool first = true;
  return tag_decode_oid<unsigned>(in, [&os, &first](unsigned i) {
    if (not first)
      os << '.';
    os << i;
    first = false;
  });
}

bool tag_content_utc_time(octet_input& in, ostream& os) {
  // return get_utc_time(in, [&os](const iso8601_time_t& t) {});
  return tag_decode_print_string<char>(in, [&os](char c) { os << c; });
}

bool(* const known_tag_content_fn[])(octet_input&, ostream&) = {
  &tag_content_octet_string, // EOC
  &tag_content_boolean,
  &tag_content_integer,
  &tag_content_bit_string,
  &tag_content_octet_string, // OCTET STRING
  &tag_content_octet_string, // NULL
  &tag_content_oid,
  &tag_content_octet_string, // Object Descriptor
  &tag_content_octet_string, // EXTERNAL
  &tag_content_octet_string, // REAL (float)
  &tag_content_octet_string, // ENUMERATED
  &tag_content_octet_string, // EMBEDDED PDV
  &tag_content_utf8_string,
  &tag_content_octet_string, // RELATIVE-OID
  &tag_content_octet_string, // TIME
  &tag_content_octet_string, // UNIVERSAL 15
  &tag_content_octet_string, // SEQUENCE (OF)
  &tag_content_octet_string, // SET (OF)
  &tag_content_octet_string, // NumericString
  &tag_content_print_string,
  &tag_content_octet_string, // T61String
  &tag_content_octet_string, // VideotexString
  &tag_content_octet_string, // IA5String
  &tag_content_utc_time,
  &tag_content_octet_string, // GeneralizedTime
  &tag_content_octet_string, // GraphicString
  &tag_content_octet_string, // VisibleString
  &tag_content_octet_string, // GeneralString
  &tag_content_octet_string, // UniversalString
  &tag_content_octet_string, // CHARACTER STRING
  &tag_content_octet_string, // BMPString
  &tag_content_octet_string, // DATE
  &tag_content_octet_string, // TIME-OF-DAY
  &tag_content_octet_string, // DATE-TIME
  &tag_content_octet_string, // DURATION
  &tag_content_octet_string, // OID-IRI
  &tag_content_octet_string, // RELATIVE-OID-IRI
};

bool tree_scan(delimited_input& in, ostream& os, int marg) {
  tag_header header;
  tag_decode_result const tdr = tag_decode(header, in);
  delimited_input::delimit_sentry const delimit_sentry(in, header.length);
  switch (tdr) {
    case tdr_empty:
      return false;
    case tdr_incomplete:
      os << "--- incomplete ---" << endl;
      return false;
    case tdr_overflow:
      os << "--- overflow ---" << endl;
      return false;
    case tdr_invalid:
      os << "--- invalid ---" << endl;
      return false;
  }
  fill_n(ostream_iterator<char>(os), marg, ' ');
  switch (header.ident.tclass) {
    case tc_universal:
      if (header.ident.tnumber < known_tag_count)
        os << known_tag_label[header.ident.tnumber];
      else
        os << "UNIVERSAL " << header.ident.tnumber;
      break;
    case tc_application:
      os << "APPLICATION " << header.ident.tnumber;
      break;
    case tc_context_specific:
      os << "CONTEXT SPECIFIC " << header.ident.tnumber;
      break;
    case tc_private:
      os << "PRIVATE " << header.ident.tnumber;
      break;
  }
  if (not header.length.definite)
    os << " ~";
  if (header.ident.constructed) {
    os << endl;
    while (tree_scan(in, os, marg + 2));
    return true;
  }
  os << " [";
  bool good = header.ident.tclass == tc_universal and header.ident.tnumber < known_tag_count
      ? known_tag_content_fn[header.ident.tnumber](in, os)
      : tag_content_octet_string(in, os);
  os << "]" << endl;
  return good and header.ident;
}

void tree_scan(octet_input& in, ostream& os) {
  delimited_input delim_in(in);
  while (tree_scan(delim_in, os, 0));
}
