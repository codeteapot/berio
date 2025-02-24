#include <iomanip>
#include <iostream>
#include <sstream>

#include <berio/b64istream.h>
#include <berio/deliminput.h>

using namespace std;
using namespace ber;

struct getn_result {
  unsigned char const* const octets;
  size_t len;
};

template<typename CharT, typename Traits>
basic_ostream<CharT, Traits>& operator << (
    basic_ostream<CharT, Traits>& os,
    getn_result const& gr) {
  typename basic_ostream<CharT, Traits>::sentry s(os);
  if (not s)
    return os;
  auto const flags = os.flags();
  os << setw(2) << setfill(os.widen('0')) << hex;
  for (size_t i = 0; i < gr.len; ++i)
    os << static_cast<unsigned int>(gr.octets[i]) << os.widen(' ');
  os.flags(flags);
  return os;
}

int main() {
  stringstream s("KmgM4fF5Kw=="); // 2a 68 0c e1 f1 79 2b
  b64_input in(s);
  delimited_input delim_in(in);
  unsigned char octets[10];
  
  /* remaining octets scope */ {
    delimited_input::sentry delimit(delim_in, 2, true); // 2a 68
    cout << "#1: " << getn_result{ octets, delim_in.getn(octets, 5) } << endl;
  }
  /* skipped octets scope */ {
    delimited_input::sentry delimit(delim_in, 4, true); // 0c e1 f1 79
    cout << "#2: " << getn_result{ octets, delim_in.getn(octets, 3) } << endl;
  }
  // 2b; the octet '79' was skipped
  cout << "#3: " << getn_result{ octets, delim_in.getn(octets, 10) } << endl;
  
  return EXIT_SUCCESS;
}
