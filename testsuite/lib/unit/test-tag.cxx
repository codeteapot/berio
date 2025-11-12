#include <cstdlib>

#include <berio/tag.h>

#include <arrayio.h>
#include <dejagnu.h>

using namespace ber;

void test_class_universal(TestState& test) {
  if (tc_universal not_eq 0) {
    test.fail("Class UNIVERSAL: Bad enum value");
    return;
  }
  test.pass("Class UNIVERSAL: Passed");
}

void test_class_application(TestState& test) {
  if (tc_application not_eq 1) {
    test.fail("Class APPLICATION: Bad enum value");
    return;
  }
  test.pass("Class APPLICATION: Passed");
}

void test_class_context_specific(TestState& test) {
  if (tc_context_specific not_eq 2) {
    test.fail("Class CONTEXT SPECIFIC: Bad enum value");
    return;
  }
  test.pass("Class CONTEXT SPECIFIC: Passed");
}

void test_class_private(TestState& test) {
  if (tc_private not_eq 3) {
    test.fail("Class PRIVATE: Bad enum value");
    return;
  }
  test.pass("Class PRIVATE: Passed");
}

void test_decode_class_universal(TestState& test) {
  array_octet_input in({ 0b00000000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode class universal: Incomplete");
    return;
  }
  if (th.tclass not_eq tc_universal) {
    test.fail("Decode class universal: Bad class");
    return;
  }
  test.pass("Decode class universal: Passed");
}

void test_decode_class_application(TestState& test) {
  array_octet_input in({ 0b01000000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode class application: Incomplete");
    return;
  }
  if (th.tclass not_eq tc_application) {
    test.fail("Decode class application: Bad class");
    return;
  }
  test.pass("Decode class application: Passed");
}

void test_decode_class_context_specific(TestState& test) {
  array_octet_input in({ 0b10000000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode class context specific: Incomplete");
    return;
  }
  if (th.tclass not_eq tc_context_specific) {
    test.fail("Decode class context specific: Bad class");
    return;
  }
  test.pass("Decode class context specific: Passed");
}

void test_decode_class_private(TestState& test) {
  array_octet_input in({ 0b11000000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode class private: Incomplete");
    return;
  }
  if (th.tclass not_eq tc_private) {
    test.fail("Decode class private: Bad class");
    return;
  }
  test.pass("Decode class private: Passed");
}

void test_decode_primitive(TestState& test) {
  array_octet_input in({ 0b00000000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode primitive: Incomplete");
    return;
  }
  if (th.shape not_eq ts_primitive) {
    test.fail("Decode primitive: Bad shape");
    return;
  }
  test.pass("Decode shape primitive: Passed");
}

void test_decode_constructed_definite(TestState& test) {
  array_octet_input in({ 0b00100000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode constructed definite: Incomplete");
    return;
  }
  if (th.shape not_eq ts_constructed_definite) {
    test.fail("Decode constructed definite: Bad shape");
    return;
  }
  test.pass("Decode constructed definite: Passed");
}

void test_decode_constructed_indefinite(TestState& test) {
  array_octet_input in({ 0b00100000, 0b10000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode constructed indefinite: Incomplete");
    return;
  }
  if (th.shape not_eq ts_constructed_indefinite) {
    test.fail("Decode constructed indefinite: Bad shape");
    return;
  }
  test.pass("Decode constructed indefinite: Passed");
}

void test_decode_min_low_number(TestState& test) {
  array_octet_input in({ 0b00000000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode min low number: Incomplete");
    return;
  }
  if (th.number not_eq 0) {
    test.fail("Decode min low number: Bad value");
    return;
  }
  test.pass("Decode min low number: Passed");
}

void test_decode_max_low_number(TestState& test) {
  array_octet_input in({ 0b00011110, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode max low number: Incomplete");
    return;
  }
  if (th.number not_eq 30) {
    test.fail("Decode max low number: Bad value");
    return;
  }
  test.pass("Decode max low number: Passed");
}

void test_decode_min_high_number(TestState& test) {
  array_octet_input in({ 0b00011111, 0b00011111, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode min high number: Incomplete");
    return;
  }
  if (th.number not_eq 31) {
    test.fail("Decode min high number: Bad value");
    return;
  }
  test.pass("Decode min high number: Passed");
}

void test_decode_large_high_number(TestState& test) {
  array_octet_input in({ 0b00011111, 0b10000001, 0b00000000, 0b00000000 });
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode large high number: Incomplete");
    return;
  }
  if (th.number not_eq 128) {
    test.fail("Decode large high number: Bad value");
    return;
  }
  test.pass("Decode large high number: Passed");
}

void test_encode_class_universal(TestState& test) {
  unsigned char arr[2];
  array_octet_output out(arr);
  
  tag_header const th { tc_universal, 0, ts_primitive, 0 };
  tag_encode(th, out);
  
  if (out.getpos() not_eq 2) {
    test.fail("Encode class universal: Bad position");
    return;
  }
  if (arr[0] not_eq 0b00000000) {
    test.fail("Encode class universal: Bad byte #0");
    return;
  }
  test.pass("Encode class universal: Passed");
}

void test_encode_class_application(TestState& test) {
  unsigned char arr[2];
  array_octet_output out(arr);
  
  tag_header const th { tc_application, 0, ts_primitive, 0 };
  tag_encode(th, out);
  
  if (out.getpos() not_eq 2) {
    test.fail("Encode class application: Bad position");
    return;
  }
  if (arr[0] not_eq 0b01000000) {
    test.fail("Encode class application: Bad byte #0");
    return;
  }
  test.pass("Encode class application: Passed");
}

void test_encode_class_context_specific(TestState& test) {
  unsigned char arr[2];
  array_octet_output out(arr);
  
  tag_header const th { tc_context_specific, 0, ts_primitive, 0 };
  tag_encode(th, out);
  
  if (out.getpos() not_eq 2) {
    test.fail("Encode class context specific: Bad position");
    return;
  }
  if (arr[0] not_eq 0b10000000) {
    test.fail("Encode class context specific: Bad byte #0");
    return;
  }
  test.pass("Encode class context specific: Passed");
}

void test_encode_class_private(TestState& test) {
  unsigned char arr[2];
  array_octet_output out(arr);
  
  tag_header const th { tc_private, 0, ts_primitive, 0 };
  tag_encode(th, out);
  
  if (out.getpos() not_eq 2) {
    test.fail("Encode class private: Bad position");
    return;
  }
  if (arr[0] not_eq 0b11000000) {
    test.fail("Encode class private: Bad byte #0");
    return;
  }
  test.pass("Encode class private: Passed");
}

int main() {
  TestState test;
  
  test_class_universal(test);
  test_class_application(test);
  test_class_context_specific(test);
  test_class_private(test);
  
  test_decode_class_universal(test);
  test_decode_class_application(test);
  test_decode_class_context_specific(test);
  test_decode_class_private(test);
  test_decode_primitive(test);
  test_decode_constructed_definite(test);
  test_decode_constructed_indefinite(test);
  test_decode_min_low_number(test);
  test_decode_max_low_number(test);
  test_decode_min_high_number(test);
  test_decode_large_high_number(test);
  
  test_encode_class_universal(test);
  test_encode_class_application(test);
  test_encode_class_context_specific(test);
  test_encode_class_private(test);
  
  return EXIT_SUCCESS;
}
