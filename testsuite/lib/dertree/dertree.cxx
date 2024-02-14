#include "dertree.h"

using namespace ber;

der_tree::der_tree(int level):
__level(level),
__tdr(tdr_empty) {}

bool der_tree::empty() const {
  return __tdr == tdr_empty;
}

bool der_tree::end() const {
  return not __header.ident;
}

bool der_tree::error() const {
  return __tdr not_eq tdr_complete or __indefinite_primitive();
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
  // TODO ...
}

void der_tree::decode(octet_input& in) {
  __tdr = tag_decode(__header, in);
}

bool der_tree::__indefinite_primitive() const {
  return not __header.ident.constructed and not __header.length.definite;
}
