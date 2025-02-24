#include "berlabel.h"
#include "berlevel.h"

using namespace ber;

ber_level::ber_level(unsigned int amount):
__amount(amount) {}

bool ber_level::operator == (ber_level const& level) const {
  return __amount == level.__amount;
}

ber_level ber_level::operator + (unsigned int amount) const {
  return __amount + amount;
}

ber_label::ber_label(tag_class tclass, tag_number number, bool definite):
__tclass(tclass),
__number(number),
__definite(definite) {}

tag_class ber_label::tclass() const {
  return __tclass;
}

tag_number ber_label::number() const {
  return __number;
}

bool ber_label::definite() const {
  return __definite;
}
