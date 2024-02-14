#include <algorithm>
#include <cstring>
#include <iostream>

#include <berio/b64istream.h>
#include <berio/b64ostream.h>

#include "derscan.h"
#include "derprint.h"

using namespace std;
using namespace ber;

bool arg_exist(int, char*[], char const*);
char const* basename_get(char*);

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
