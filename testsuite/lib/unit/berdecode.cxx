#include <cstdlib>

#include <dejagnu.h>

int main() {
  TestState state;
  state.pass("Test passing");
  return EXIT_SUCCESS;
}
