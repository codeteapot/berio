#include <cstdlib>
#include <limits>

#include <berio/tag.h>

#include <dejagnu.h>
#include <testio.h>

using namespace ber;
using namespace std;

void test_decode_class_universal(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b01000000, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b10000000, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b11000000, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b00000000, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b00100000, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b00100000, 0b10000000));
  
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
  array_octet_input in(make_octet_array(0b00000000, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b00011110, 0b00000000));
  
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
  array_octet_input in(make_octet_array(0b00011111, 0b00011111, 0b00000000));
  
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

void test_decode_max_high_number(TestState& test) {
  array_octet_input in(make_octet_array(
    0b00011111,
    []() {
      constexpr size_t r = numeric_limits<tag_number>::digits % 7;
      constexpr size_t n = numeric_limits<tag_number>::digits / 7 + (r == 0 ? 0 : 1);
      octet_array<n> a{};
      a[0] = 0xff >> (8 - r) bitor (n == 3 ? 0x00 : 0x80);
      fill(begin(a) + 1, end(a) - 1, 0xff);
      a[n - 1] = 0x7f;
      return a;
    },
    0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode max high number: Incomplete");
    return;
  }
  if (th.number not_eq numeric_limits<tag_number>::max()) {
    test.fail("Decode max high number: Bad value");
    return;
  }
  test.pass("Decode max high number: Passed");
}

void test_decode_min_short_length(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode min short length: Incomplete");
    return;
  }
  if (th.length not_eq 0) {
    test.fail("Decode min short length: Bad value");
    return;
  }
  if (th.shape == ts_constructed_indefinite) { // Not because it is indefinite
    test.fail("Decode min short length: Bad shape");
    return;
  }
  test.pass("Decode min short length: Passed");
}

void test_decode_max_short_length(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b01111111));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode max short length: Incomplete");
    return;
  }
  if (th.length not_eq 127) {
    test.fail("Decode max short length: Bad value");
    return;
  }
  test.pass("Decode max short length: Passed");
}

void test_decode_min_long_length(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b10000001, 0b10000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode min long length: Incomplete");
    return;
  }
  if (th.length not_eq 128) {
    test.fail("Decode min long length: Bad value");
    return;
  }
  test.pass("Decode min long length: Passed");
}

void test_decode_max_long_length(TestState& test) {
  array_octet_input in(make_octet_array(
    0b00000000,
    []() {
      constexpr size_t r = numeric_limits<size_t>::digits % 8;
      constexpr size_t s = numeric_limits<size_t>::digits / 8 + (r == 0 ? 0 : 1);
      constexpr size_t n = s + 1;
      octet_array<n> a{};
      a[0] = 0x80 bitor s;
      a[1] = 0xff >> r;
      fill(begin(a) + 2, end(a), 0xff);
      return a;
    }));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode max long length: Incomplete");
    return;
  }
  if (th.length not_eq numeric_limits<size_t>::max()) {
    test.fail("Decode max long length: Bad value");
    return;
  }
  test.pass("Decode max long length: Passed");
}

// TODO Add idempotence check in non complete decoding tests

void test_decode_empty(TestState& test) {
  array_octet_input in(make_octet_array());
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_empty) {
    test.fail("Decode empty: Not empty");
    return;
  }
  test.pass("Decode empty: Passed");
}

void test_decode_number_incomplete_first(TestState& test) {
  array_octet_input in(make_octet_array(0b00011111));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_number_incomplete) {
    test.fail("Decode number incomplete (first): Not number incomplete");
    return;
  }
  test.pass("Decode number incomplete (first): Passed");
}

void test_decode_number_incomplete_following(TestState& test) {
  array_octet_input in(make_octet_array(0b00011111, 0b10000001));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_number_incomplete) {
    test.fail("Decode number incomplete (following): Not number incomplete");
    return;
  }
  test.pass("Decode number incomplete (following): Passed");
}

void test_decode_number_zero_leading(TestState& test) {
  array_octet_input in(make_octet_array(0b00011111, 0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_number_zero_leading) {
    test.fail("Decode number zero leading: Not number zero leading");
    return;
  }
  test.pass("Decode number zero leading: Passed");
}

void test_decode_number_zero_leading_no_strict(TestState& test) {
  array_octet_input in(make_octet_array(0b00011111, 0b00000000, 0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in, false);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode number zero leading no strict: Incomplete");
    return;
  }
  test.pass("Decode number zero leading no strict: Passed");
}

void test_decode_number_overflow(TestState& test) {
  array_octet_input in(make_octet_array(
    0b00011111,
    []() {
      constexpr size_t r = numeric_limits<tag_number>::digits % 7;
      constexpr size_t n = numeric_limits<tag_number>::digits / 7 + (r == 0 ? 1 : 0) + 1;
      octet_array<n> a{};
      a[0] = 0x80 bitor 0x01 << r;
      fill(begin(a) + 1, end(a) - 1, 0x80);
      a[n - 1] = 0x00;
      return a;
    },
    0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_number_overflow) {
    test.fail("Decode number overflow: Not number overflow");
    return;
  }
  test.pass("Decode number overflow: Passed");
}

void test_decode_number_unneeded_high(TestState& test) {
  array_octet_input in(make_octet_array(0b00011111, 0b00011110, 0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_number_unneeded_high) {
    test.fail("Decode number unneeded high: Not number unneeded high");
    return;
  }
  test.pass("Decode number unneeded high: Passed");
}

void test_decode_number_unneeded_high_no_strict(TestState& test) {
  array_octet_input in(make_octet_array(0b00011111, 0b00011110, 0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in, false);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode number unneeded high no strict: Incomplete");
    return;
  }
  test.pass("Decode number unneeded high no strict: Passed");
}

void test_decode_length_undefined(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_length_undefined) {
    test.fail("Decode length undefined: Not length undefined");
    return;
  }
  test.pass("Decode length undefined: Passed");
}

void test_decode_length_unsupported(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b11111111));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_length_unsupported) {
    test.fail("Decode length unsupported: Not length unsupported");
    return;
  }
  test.pass("Decode length unsupported: Passed");
}

void test_decode_indefinite_primitive(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b10000000));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_indefinite_primitive) {
    test.fail("Decode indefinite primitive: Not indefinite primitive");
    return;
  }
  test.pass("Decode indefinite primitive: Passed");
}

void test_decode_length_incomplete(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b10000001));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_length_incomplete) {
    test.fail("Decode length incomplete: Not length incomplete");
    return;
  }
  test.pass("Decode length incomplete: Passed");
}

void test_decode_length_overflow(TestState& test) {
  array_octet_input in(make_octet_array(
    0b00000000,
    []() {
      constexpr size_t r = numeric_limits<size_t>::digits % 8;
      constexpr size_t s = numeric_limits<size_t>::digits / 8 + (r == 0 ? 1 : 0);
      constexpr size_t n = s + 1;
      octet_array<n> a{};
      a[0] = 0x80 bitor s;
      a[1] = 0x01 << r;
      fill(begin(a) + 2, end(a), 0x00);
      return a;
    }));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_length_overflow) {
    test.fail("Decode length overflow: Not length overflow");
    return;
  }
  test.pass("Decode length overflow: Passed");
}

void test_decode_length_unneeded_long(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b10000001, 0b01111111));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in);
  
  if (tr not_eq tdr_length_unneeded_long) {
    test.fail("Decode length unneeded long: Not length unneeded long");
    return;
  }
  test.pass("Decode length unneeded long: Passed");
}

void test_decode_length_unneeded_long_no_strict(TestState& test) {
  array_octet_input in(make_octet_array(0b00000000, 0b10000001, 0b01111111));
  
  tag_header th;
  tag_decode_result tr = tag_decode(th, in, false);
  
  if (tr not_eq tdr_complete) {
    test.fail("Decode length unneeded long no strict: Incomplete");
    return;
  }
  test.pass("Decode length unneeded long no strict: Passed");
}

void test_sizeof_min_low_number(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, 0, ts_primitive, 0 });
  
  if (s not_eq 2) {
    test.fail("Size of min low number: Bad value");
    return;
  }
  test.pass("Size of min low number: Passed");
}

void test_sizeof_max_low_number(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, 30, ts_primitive, 0 });
  
  if (s not_eq 2) {
    test.fail("Size of max low number: Bad value");
    return;
  }
  test.pass("Size of max low number: Passed");
}

void test_sizeof_min_high_number(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, 31, ts_primitive, 0 });
  
  if (s not_eq 3) {
    test.fail("Size of min high number: Bad value");
    return;
  }
  test.pass("Size of min high number: Passed");
}

void test_sizeof_max_high_number(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, numeric_limits<tag_number>::max(), ts_primitive, 0 });
  
  if (s not_eq (numeric_limits<tag_number>::digits + 6) / 7 + 2) {
    test.fail("Size of max high number: Bad value");
    return;
  }
  test.pass("Size of max high number: Passed");
}

void test_sizeof_min_short_length(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, 0, ts_primitive, 0 });
  
  if (s not_eq 2) {
    test.fail("Size of min short length: Bad value");
    return;
  }
  test.pass("Size of min short length: Passed");
}

void test_sizeof_max_short_length(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, 0, ts_primitive, 127 });
  
  if (s not_eq 2) {
    test.fail("Size of max short length: Bad value");
    return;
  }
  test.pass("Size of max short length: Passed");
}

void test_sizeof_min_long_length(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, 0, ts_primitive, 128 });
  
  if (s not_eq 3) {
    test.fail("Size of min long length: Bad value");
    return;
  }
  test.pass("Size of min long length: Passed");
}

void test_sizeof_max_long_length(TestState& test) {
  size_t s = tag_sizeof({ tc_universal, 0, ts_primitive, numeric_limits<size_t>::max() });
  
  if (s not_eq (numeric_limits<size_t>::digits + 7) / 8 + 2) {
    test.fail("Size of max long length: Bad value");
    return;
  }
  test.pass("Size of max long length: Passed");
}

void test_sizeof_indefinite_length(TestState& test) {
  size_t s = tag_sizeof({
    tc_universal,
    0,
    ts_constructed_indefinite,
    numeric_limits<size_t>::max()
  });
  
  if (s not_eq 2) {
    test.fail("Size of indefinite length: Bad value");
    return;
  }
  test.pass("Size of indefinite length: Passed");
}

void test_encode_class_universal(TestState& test) {
  octet_array<2> arr;
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
  octet_array<2> arr;
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
  octet_array<2> arr;
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
  octet_array<2> arr;
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
  test_decode_max_high_number(test);
  test_decode_min_short_length(test);
  test_decode_max_short_length(test);
  test_decode_min_long_length(test);
  test_decode_max_long_length(test);
  test_decode_empty(test);
  test_decode_number_incomplete_first(test);
  test_decode_number_incomplete_following(test);
  test_decode_number_zero_leading(test);
  test_decode_number_zero_leading_no_strict(test);
  test_decode_number_overflow(test);
  test_decode_number_unneeded_high(test);
  test_decode_number_unneeded_high_no_strict(test);
  test_decode_length_undefined(test);
  test_decode_length_unsupported(test);
  test_decode_indefinite_primitive(test);
  test_decode_length_incomplete(test);
  test_decode_length_overflow(test);
  test_decode_length_unneeded_long(test);
  test_decode_length_unneeded_long_no_strict(test);
  
  test_sizeof_min_low_number(test);
  test_sizeof_max_low_number(test);
  test_sizeof_min_high_number(test);
  test_sizeof_max_high_number(test);
  test_sizeof_min_short_length(test);
  test_sizeof_max_short_length(test);
  test_sizeof_min_long_length(test);
  test_sizeof_max_long_length(test);
  test_sizeof_indefinite_length(test);
  
  test_encode_class_universal(test);
  test_encode_class_application(test);
  test_encode_class_context_specific(test);
  test_encode_class_private(test);
  
  return EXIT_SUCCESS;
}
