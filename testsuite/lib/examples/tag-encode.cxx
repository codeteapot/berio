#include <iostream>

#include <berio/b64ostream.h>
#include <berio/tag.h>

using namespace std;
using namespace ber;

int main() {
  b64_output out(cout);
  
  cout << "Result: ";
  if (tag_encode({ tc_private, 42, ts_primitive, 195 }, out))
    out.conclude();
  else
    cout << " (incomplete)";
  cout << endl;
  return EXIT_SUCCESS;
}
