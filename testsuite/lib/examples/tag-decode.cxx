#include <iostream>

#include <berio/b64sstream.h>
#include <berio/tag.h>

using namespace std;
using namespace ber;

int main() {
  stringstream s("3yqBww=="); // df 2a 81 c3
  b64_input in(s);
  
  tag_header th;
  if (tag_decode(th, in) == tdr_complete) {
    cout << "Universal: " << boolalpha << (th.tclass == tc_universal) << endl;
    cout << "Number: " << th.number << endl;
    cout << "Primitive: " << boolalpha << (th.shape == ts_primitive) << endl;
    cout << "Length: " << th.length << endl;
  }
  return EXIT_SUCCESS;
}
