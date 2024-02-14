#include <iterator>

#include "derformat.h"

using namespace std;

tree_level const tree_level::error(__error_amount);

tree_level::tree_level(int amount):
__amount(amount) {
}

tree_level::tree_level(tree_level const& level):
tree_level(level.__amount) {
}

tree_level tree_level::operator ++ () const {
  return tree_level(__amount + 1);
}

bool tree_level::operator == (int amount) const {
  return __amount == amount;
}

bool tree_level::__error() const {
  return __amount == __error_amount;
}

istream& operator >> (istream& is, tree_level& level) {
  int marg = 0;
  int c = is.peek();
  if (not is.good() or c == '\n')
    return is.setstate(is.rdstate() bitor ios::failbit), is;
  while (c == ' ') {
    is.ignore();
    ++marg;
    c = is.peek();
    if (not is.good() or c == '\n')
      return is.setstate(is.rdstate() bitor ios::badbit), is;
  }
  if (marg % 2 == 0)
    level.__amount = marg / 2;
  else
    is.setstate(is.rdstate() bitor ios::badbit);
  return is;
}

ostream& operator << (ostream& os, tree_level const& level) {
  if (level.__error()) {
    os << "!";
    os.setstate(os.rdstate() bitor ios::failbit);
    return os;
  }
  fill_n(ostream_iterator<char>(os), level.__amount * level.__margin_size, ' ');
  return os;
}
