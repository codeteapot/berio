#include <cstdlib>

#include <berio/types.h>

#include <arrayio.h>
#include <dejagnu.h>

using namespace ber;

void test_decode_false(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000));
  bool b = true;
  
  tag_decode_boolean_result tdbr = tag_decode_boolean(b, in);
  
  if (tdbr not_eq tdbr_complete) {
    test.fail("Decode type boolean: Incomplete");
    return;
  }
  if (b) {
    test.fail("Decode type boolean: Bad value");
    return;
  }
  test.pass("Decode type boolean: Passed");
}

void test_decode_true(TestState& test) {
  array_octet_input in(make_octet_array(0b11111111));
  bool b = false;
  
  tag_decode_boolean_result tdbr = tag_decode_boolean(b, in);
  
  if (tdbr not_eq tdbr_complete) {
    test.fail("Decode type boolean: Incomplete");
    return;
  }
  if (not b) {
    test.fail("Decode type boolean: Bad value");
    return;
  }
  test.pass("Decode type boolean: Passed");
}

void test_decode_not_false(TestState& test) {
  array_octet_input in(make_octet_array(0b00000001));
  bool b = false;
  
  tag_decode_boolean_result tdbr = tag_decode_boolean(b, in);
  
  if (tdbr not_eq tdbr_complete) {
    test.fail("Decode type boolean: Incomplete");
    return;
  }
  if (not b) {
    test.fail("Decode type boolean: Bad value");
    return;
  }
  test.pass("Decode type boolean: Passed");
}

void test_decode_empty(TestState& test) {
  array_octet_input in(make_octet_array());
  bool b = false;
  
  tag_decode_boolean_result tdbr = tag_decode_boolean(b, in);
  
  if (tdbr not_eq tdbr_empty) {
    test.fail("Decode type boolean: Complete");
    return;
  }
  test.pass("Decode type boolean: Passed");
}

int main() {
  TestState test;
  try {
    test_decode_false(test);
    test_decode_true(test);
    test_decode_not_false(test);
    test_decode_empty(test);
  }
  catch (...) {
    test.fail("Something went wrong...");
  }
  return EXIT_SUCCESS;
}
