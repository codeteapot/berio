#include <iostream>

#include <berio/tag.h>

using namespace std;
using namespace ber;

int main() {
  cout << "Size: " << tag_sizeof({ tc_private, 42, ts_primitive, 195 }) << endl;
  return EXIT_SUCCESS;
}
