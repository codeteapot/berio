#include <algorithm>
#include <cstring>
#include <iostream>

#include <berio/deliminput.h>
#include <berio/b64istream.h>
#include <berio/b64ostream.h>

#include "dertree.h"

using namespace std;
using namespace ber;

bool arg_exist(int, char*[], char const*);
char const* basename_get(char*);

enum der_scan_result {
  dsr_remaining,
  dsr_finished,
  dsr_failed
};

template<typename CharT, typename Traits>
der_scan_result der_scan(delimited_input& in, basic_ostream<CharT, Traits>& os, int level) {
  der_tree tree(level);
  tree.scan(in);
  if (tree.empty())
    return dsr_finished;
  os << tree;
  if (not tree.complete())
    return dsr_failed;
  return os.good() ? tree.end() ? dsr_finished : dsr_remaining : dsr_failed;
}

enum der_print_result {
  dpr_finished,
  dpr_failed
};

template<typename CharT, typename Traits>
der_print_result der_print(octet_output& out, basic_istream<CharT, Traits>& is, int level) {
  return dpr_failed;
}

int main(int argc, char* argv[]) {
  if (arg_exist(argc, argv, "-s")) {
    b64_istream_source b64_cin_src(cin);
    b64_input b64_cin(b64_cin_src);
    delimited_input delim_b64_cin(b64_cin);
    der_scan(delim_b64_cin, cout, 0);
    return EXIT_SUCCESS;
  }
  if (arg_exist(argc, argv, "-p")) {
    b64_ostream_target b64_cout_tgt(cout);
    b64_output b64_cout(b64_cout_tgt);
    der_print(b64_cout, cin, 0);
    b64_cout.conclude();
    return EXIT_SUCCESS;
  }
  cout << "Usage: " << basename_get(argv[0]) << " (-s|-p)" << endl;
  return EXIT_FAILURE;
}

bool arg_exist(int argc, char* argv[], char const* argn) {
  return find_if(
    argv + 1,
    argv + argc,
    [argn] (char const* str) { return strcmp(str, argn) == 0; }) not_eq argv + argc;
}

char const* basename_get(char* path) {
  char const* c_name = strrchr(path, '/');
  return c_name == nullptr ? path : c_name + 1; // PRE Not ended with '/'
}
