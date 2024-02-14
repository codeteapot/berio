#include <berio/io.h>

using namespace ber;

octet_input::~octet_input() {}
  
bool octet_input::get(unsigned char& o) {
  return get(&o, 1) > 0;
}

octet_output::~octet_output() {}
  
bool octet_output::put(unsigned char o) {
  return put(&o, 1) > 0;
}
