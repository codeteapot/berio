#include <berio/univ.h>

#include "berformat.h"

using namespace std;
using namespace ber;

constexpr char const* const univ_tag_label_text[] {
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

tree_label::tree_label():
tclass(tc_universal),
tnumber(univ::tn_eoc) {}

ostream& operator << (ostream& os, tree_label const& label) {
  switch (label.tclass) {
    case tc_universal:
      if (label.tnumber < sizeof (univ_tag_label_text))
        os << univ_tag_label_text[label.tnumber];
      else
        os << "UNIVERSAL " << label.tnumber;
      break;
    case tc_application:
      os << "APPLICATION " << label.tnumber;
      break;
    case tc_context_specific:
      os << "CONTEXT SPECIFIC " << label.tnumber;
      break;
    case tc_private:
      os << "PRIVATE " << label.tnumber;
      break;
  }
  return os;
}
