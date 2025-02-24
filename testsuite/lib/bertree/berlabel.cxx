#include "bertree.h"

using namespace ber;

ber_label::ber_label(tag_class const& tclass, tag_number const& number, bool definite):
__tclass(tclass),
__number(number),
__definite(definite) {}

tag_class const& ber_label::tclass() const {
  return __tclass;
}

tag_number const& ber_label::number() const {
  return __number;
}

bool ber_label::definite() const {
  return __definite;
}
