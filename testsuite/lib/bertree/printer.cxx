#include "berformat.h"
#include "bertree.h"

using namespace std;
using namespace ber;

void tree_print(octet_output& out, istream& is) {
  tree_level level;
  is >> level;
  if (level == 0)
    ; // TODO ...
  else
    is.setstate(is.rdstate() bitor ios::badbit);
}
