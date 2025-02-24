#include <cstdlib>
#include <iostream>

#include <berio/tag.h>

using namespace std;
using namespace ber;

//! [Is constructed]
bool is_constructed(tag_header const& th) {
  return th.shape not_eq ts_primitive;
}
//! [Is constructed]

//! [Is definite]
bool is_definite(tag_header const& th) {
  return th.shape not_eq ts_constructed_indefinite;
}
//! [Is definite]

int main() {
  tag_header th { tc_private, 0, ts_constructed_definite, 0 };
  cout << boolalpha;
  cout << "Constructed: " << is_constructed(th) << endl;
  cout << "Definite: " << is_definite(th) << endl;
  return EXIT_SUCCESS;
}
