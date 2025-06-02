#include <berio/univ.h>

#include "berprint.h"

using namespace ber;
using namespace ber::univ;
using namespace std;

ber_printer::ber_printer():
__node(nullptr) {}

ber_printer::~ber_printer() {
  if (__node not_eq nullptr)
    delete __node;
}

ber_level const& ber_printer::level() const {
  return __level;
}

bool ber_printer::end() const {
  return __node == nullptr or
    __tclass == tc_universal and
    __number == tn_eoc and
    __node->shape() == ts_primitive;
}
  
void ber_printer::print(octet_output& out) const {
  if (__node not_eq nullptr) {
    tag_encode({ __tclass, __number, __node->shape(), __node->length() }, out);
    __node->print(out);
  }
}

void ber_printer::__replace(
    ber_level const& level,
    tag_class tclass,
    tag_number number,
    ber_printer_node* node) {
  __level = level;
  __tclass = tclass;
  __number = number;
  if (__node not_eq nullptr)
    delete __node;
  __node = node;
}

ber_printer_node::~ber_printer_node() {}
