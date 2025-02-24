#include <iostream>

#include <berio/b64ostream.h>
#include <berio/tag.h>

using namespace std;
using namespace ber;

int main() {
  b64_output out(cout);
  
  cout << "Result: ";
  if (not tag_encode({ tc_private, 42, ts_primitive, 195 }, out))
    cout << " (incomplete)";
  out.flush();
  cout << endl;
  return EXIT_SUCCESS;
}
