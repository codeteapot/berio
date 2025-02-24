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

ber_printer_node const* ber_printer::operator -> () const {
  return __node; // May be nullptr
}

ber_printer_node* ber_printer::detach() {
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

ber_printer_node::ber_printer_node(ber::tag_header const& th):
__th(th) {}

ber_printer_node::~ber_printer_node() {}

bool ber_printer_node::end() const {
  return __th == th_end;
}

void ber_printer_node::print(octet_output& out) const {
  tag_encode(__th, out);
  content_print(out);
}

size_t ber_printer_node::length() const {
  return tag_sizeof(__th) + __th.length; // TODO Cache tag_sizeof
}

ber_printer_definite::ber_printer_definite(ber::tag_class tclass, ber::tag_number number):
ber_printer_node({ tclass, number, ber::ts_constructed_definite, 0 }) {}

ber_printer_definite::~ber_printer_definite() {
  for (auto it = __children.begin(); it not_eq __children.end(); ++it)
    delete *it;
}

void ber_printer_definite::push_back(ber_printer_node* node) {
  __th.length += node->length();
  __children.push_back(node);
}

void ber_printer_definite::content_print(octet_output& out) const {
  for (auto it = __children.begin(); it not_eq __children.end(); ++it)
    (*it)->print(out);
}
