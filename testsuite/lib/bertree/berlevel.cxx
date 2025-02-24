#include "bertree.h"

using namespace ber;

ber_level::ber_level(unsigned amount):
__amount(amount) {}

ber_level ber_level::operator + (unsigned amount) const {
  return __amount + amount;
}
