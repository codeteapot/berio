#include "derformat.h"
#include "dertree.h"

using namespace std;
using namespace ber;

void tree_print(octet_output& out, istream& is) {
  tree_level new_level;
  is >> new_level;
  if (new_level == 0) {
    tree_label label;
    is >> label;
    if (is.good())
      ; // TODO ...
      // Primitive -> Definite with buffer
      // Primitive UNIVERSAL -> Definite without buffer
      // Primitive EOC -> Definite empty and decrement level
      // Constructed with EOF -> Indefinite with buffer
      // Constructed without EOF -> Definite with buffer
  }
  else
    is.setstate(is.rdstate() bitor ios::badbit);
}
