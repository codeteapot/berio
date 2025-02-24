#include <cstring>
#include <iostream>

#include <berio/b64istream.h>
#include <berio/b64ostream.h>

#include "berscan.h"
#include "berprint.h"

using namespace std;
using namespace ber;

bool arg_exist(int, char*[], char const*);
char const* basename_get(char*);

int main(int argc, char* argv[]) {
  if (arg_exist(argc, argv, "-s")) {
    b64_input in(cin);
    delimited_input delim_in(in);
    ber_scan(delim_in, cout);
    return EXIT_SUCCESS;
  }
  if (arg_exist(argc, argv, "-p")) {
    b64_output out(cout);
    ber_print(out, cin);
    out.conclude();
    cout << std::endl;
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
