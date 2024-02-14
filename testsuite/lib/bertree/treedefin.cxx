#include "berformat.h"

using namespace std;

tree_definite::tree_definite(bool value):
__value(value) {}

ostream& operator << (ostream& os, tree_definite const& defin) {
  if (not defin.__value)
    os << " ~";
  return os;
}
