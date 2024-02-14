#include "derformat.h"

using namespace std;

istream& operator >> (istream& is, tree_primitive& prim) {
  return is;
}

ostream& operator << (ostream& os, tree_primitive const& prim) {
  return os;
}
