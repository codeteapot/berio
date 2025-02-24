#include <cstdlib>
#include <sstream>

#include <berio/b64istream.h>
#include <berio/tag.h>

using namespace ber;

int main() {
  std::stringstream ss;
  b64_input in(ss);
  
  tag_header th;
  tag_decode(th, in);
  
  return EXIT_SUCCESS;
}
