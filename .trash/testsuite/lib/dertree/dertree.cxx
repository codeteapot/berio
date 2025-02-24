#include <berio/univ.h>

#include "dertree.h"

using namespace ber;
using namespace ber::univ;

der_tree::der_tree(int level):
__level(level),
__tdr(tdr_empty) {}

bool der_tree::empty() const {
  return __tdr == tdr_empty;
}

bool der_tree::complete() const {
  return __tdr == tdr_complete;
}

bool der_tree::end() const {
  return __th.tclass == tc_universal and __th.number == tn_eoc and __th.shape == ts_primitive;
}

bool der_tree::constructed() const {
  return __th.shape not_eq ts_primitive;
}

bool der_tree::definite() const {
  return __th.shape not_eq ts_constructed_indefinite;
}

size_t der_tree::length() const {
  return __th.length;
}

void der_tree::length(size_t length) {
  __th.length = length;
}

void der_tree::print(octet_output& out) const {
  tag_encode(__th, out);
}

void der_tree::scan(octet_input& in) {
  __tdr = tag_decode(__th, in);
}
