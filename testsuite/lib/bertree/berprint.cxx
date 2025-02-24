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

ber_printer_node* ber_printer::operator -> () {
  return __node; // May be nullptr
}

ber_printer_node* ber_printer::node_pop() {
  ber_printer_node* left = __node;
  __node = nullptr;
  return left;
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

void ber_printer_node::print(octet_output& out) {
  tag_encode({ __tclass, __number, shape(), content_length() }, out);
  content_print(out);
}

size_t ber_printer_node::length() {
  return tag_sizeof({ __tclass, __number, shape(), content_length() }) + content_length();
}

ber_printer_definite::ber_printer_definite(ber::tag_class tclass, ber::tag_number number):
ber_printer_node(tclass, number),
__length_calculated(false),
__length(0) {}

ber_printer_definite::~ber_printer_definite() {
  for (auto it = __children.begin(); it not_eq __children.end(); ++it)
    delete *it;
}

void ber_printer_definite::push_back(ber_printer_node* node) {
  __children.push_back(node);
}

tag_shape ber_printer_definite::shape() const {
  return ts_constructed_definite;
}

size_t ber_printer_definite::content_length() {
  if (not __length_calculated) {
    for (auto it = __children.begin(); it not_eq __children.end(); ++it)
      __length += (*it)->length();
    __length_calculated = true;
  }
  return __length;
}

void ber_printer_definite::content_print(octet_output& out) const {
  for (auto it = __children.begin(); it not_eq __children.end(); ++it)
    (*it)->print(out);
}
