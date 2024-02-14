#include "dertree.h"

using namespace ber;

der_tree::der_tree(int level):
__level(level),
__tdr(tdr_empty) {}

bool der_tree::empty() const {
  return __tdr == tdr_empty;
}

bool der_tree::complete() const {
  return __tdr == tdr_complete;
}

tag_ident const& der_tree::ident() const {
  return __header.ident;
}

tag_length const& der_tree::length() const {
  return __header.length;
}

void der_tree::length(tag_length const& len) {
  __header.length = len;
}

void der_tree::encode(octet_output& out) const {
  tag_encode(__header, out);
}

void der_tree::decode(octet_input& in) {
  __tdr = tag_decode(__header, in);
}
