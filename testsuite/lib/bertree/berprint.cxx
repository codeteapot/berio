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
  return __node == nullptr or __node->end();
}
  
void ber_printer::print(octet_output& out) const {
  if (__node not_eq nullptr)
    __node->print(out);
}

void ber_printer::__replace(ber_level const& level, ber_printer_node* node) {
  __level = level;
  if (__node not_eq nullptr)
    delete __node;
  __node = node;
}

ber_printer_node::ber_printer_node(ber::tag_class tclass, ber::tag_number number):
__tclass(tclass),
__number(number) {}

ber_printer_node::~ber_printer_node() {}

bool ber_printer_node::end() const {
  return __tclass == tc_universal and __number == tn_eoc and shape() == ts_primitive;
}

void ber_printer_node::print(octet_output& out) const {
  tag_encode({ __tclass, __number, shape(), length() }, out);
  content_print(out);
}
