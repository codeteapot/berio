#include <iostream>
#include <sstream>

#include <berio/b64istream.h>
#include <berio/tag.h>

using namespace std;
using namespace ber;

int main() {
  stringstream s("AAA=");
  //b64_input<istream, b64_source_traits<istream>> in(s);
  b64_input in(s);
  
  tag_header th;
  if (tag_decode(th, in) == tdr_complete) {
    cout << "Class:  " << th.tclass << endl;
    cout << "Number: " << th.number << endl;
    cout << "Shape:  " << th.shape << endl;
    cout << "Length: " << th.length << endl;
  }
  return EXIT_SUCCESS;
}
