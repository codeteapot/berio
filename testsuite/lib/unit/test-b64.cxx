#include <berio/b64.h>

#include <dejagnu.h>

class test_source {

  public:
  
  constexpr test_source(std::string_view);
  
  bool peek(char&);
  
  void ignore();
  
  private:
  
  std::string_view const __str;
  std::string_view::size_type __index;
};

class test_sink {

  public:
  
  test_sink(
      std::string&,
      std::string::size_type = std::numeric_limits<std::string::size_type>::max());
  
  bool put(char);
  
  private:
  
  std::string& __str;
  std::string::size_type const __max;
};

template<>
class ber::b64_source_traits<test_source> {

  public:
  
  static bool peek(test_source& src, char& c) {
    return src.peek(c);
  }
  
  static void ignore(test_source& src) {
    src.ignore();
  }
};

template<>
class ber::b64_sink_traits<test_sink> {

  public:
  
  static bool put(test_sink& sink, char c) {
    return sink.put(c);
  }
};

using namespace ber;
using namespace std;

constexpr test_source::test_source(string_view str):
__str(str),
__index(0) {}

bool test_source::peek(char& c) {
  if (__index < __str.size()) {
    c = __str[__index];
    return true;
  }
  return false;
}

void test_source::ignore() {
  if (__index < __str.size()) ++__index;
}

test_sink::test_sink(string& str, string::size_type max):
__str(str),
__max(max) {}

bool test_sink::put(char c) {
  if (__str.size() < __max) {
    __str.push_back(c);
    return true;
  }
  return false;
}

void test_input_getn_available(TestState& test) {
  test_source src("A QI\rDB\tAU\nG");
  b64_input in(src);
  unsigned char octets[7];
  
  size_t count = in.getn(octets, 7);
  
  if (count not_eq 6) {
    test.fail("Input get available: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get available: Bad octet #0");
    return;
  }
  if (octets[1] not_eq 0x02) {
    test.fail("Input get available: Bad octet #1");
    return;
  }
  if (octets[2] not_eq 0x03) {
    test.fail("Input get available: Bad octet #2");
    return;
  }
  if (octets[3] not_eq 0x04) {
    test.fail("Input get available: Bad octet #3");
    return;
  }
  if (octets[4] not_eq 0x05) {
    test.fail("Input get available: Bad octet #4");
    return;
  }
  if (octets[5] not_eq 0x06) {
    test.fail("Input get available: Bad octet #5");
    return;
  }
  test.pass("Input get available: Passed");
}

void test_input_getn_requested(TestState& test) {
  test_source src("A QI\rDB\tAU\nG");
  b64_input in(src);
  unsigned char octets[6];
  
  size_t count = in.getn(octets, 5);
  
  if (count not_eq 5) {
    test.fail("Input get requested: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get requested: Bad octet #0");
    return;
  }
  if (octets[1] not_eq 0x02) {
    test.fail("Input get requested: Bad octet #1");
    return;
  }
  if (octets[2] not_eq 0x03) {
    test.fail("Input get requested: Bad octet #2");
    return;
  }
  if (octets[3] not_eq 0x04) {
    test.fail("Input get requested: Bad octet #3");
    return;
  }
  if (octets[4] not_eq 0x05) {
    test.fail("Input get requested: Bad octet #4");
    return;
  }
  test.pass("Input get requested: Passed");
}

void test_input_getn_incomplete_first_one(TestState& test) {
  test_source src("");
  b64_input in(src);
  unsigned char octets[1];
  
  size_t count = in.getn(octets, 1);
  
  if (count not_eq 0) {
    test.fail("Input get incomplete first one: Bad count");
    return;
  }
  test.pass("Input get incomplete first one: Passed");
}

void test_input_getn_invalid_first_one(TestState& test) {
  test_source src("=");
  b64_input in(src);
  unsigned char octets[1];
  
  size_t count = in.getn(octets, 1);
  
  if (count not_eq 0) {
    test.fail("Input get invalid first one: Bad count");
    return;
  }
  test.pass("Input get invalid first one: Passed");
}

void test_input_getn_incomplete_first_two(TestState& test) {
  test_source src("A");
  b64_input in(src);
  unsigned char octets[1];
  
  size_t count = in.getn(octets, 1);
  
  if (count not_eq 0) {
    test.fail("Input get incomplete first two: Bad count");
    return;
  }
  test.pass("Input get incomplete first two: Passed");
}

void test_input_getn_invalid_first_two(TestState& test) {
  test_source src("A=");
  b64_input in(src);
  unsigned char octets[1];
  
  size_t count = in.getn(octets, 1);
  
  if (count not_eq 0) {
    test.fail("Input get invalid first two: Bad count");
    return;
  }
  test.pass("Input get invalid first two: Passed");
}

void test_input_getn_incomplete_second(TestState& test) {
  test_source src("AQ");
  b64_input in(src);
  unsigned char octets[2];
  
  size_t count = in.getn(octets, 2);
  
  if (count not_eq 1) {
    test.fail("Input get incomplete second: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get incomplete second: Bad octet #0");
    return;
  }
  test.pass("Input get incomplete second: Passed");
}

void test_input_getn_padding_second(TestState& test) {
  test_source src("AQ==");
  b64_input in(src);
  unsigned char octets[2];
  
  size_t count = in.getn(octets, 2);
  
  if (count not_eq 1) {
    test.fail("Input get available padding second: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get available padding second: Bad octet #0");
    return;
  }
  test.pass("Input get available padding second: Passed");
}

void test_input_getn_invalid_second(TestState& test) {
  test_source src("AQ@");
  b64_input in(src);
  unsigned char octets[2];
  
  size_t count = in.getn(octets, 2);
  
  if (count not_eq 1) {
    test.fail("Input get invalid second: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get invalid second: Bad octet #0");
    return;
  }
  test.pass("Input get invalid second: Passed");
}

void test_input_getn_incomplete_third(TestState& test) {
  test_source src("AQI");
  b64_input in(src);
  unsigned char octets[3];
  
  size_t count = in.getn(octets, 3);
  
  if (count not_eq 2) {
    test.fail("Input get incomplete third: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get incomplete third: Bad octet #0");
    return;
  }
  if (octets[1] not_eq 0x02) {
    test.fail("Input get incomplete third: Bad octet #1");
    return;
  }
  test.pass("Input get incomplete third: Passed");
}

void test_input_getn_padding_third(TestState& test) {
  test_source src("AQI=");
  b64_input in(src);
  unsigned char octets[3];
  
  size_t count = in.getn(octets, 3);
  
  if (count not_eq 2) {
    test.fail("Input get available padding third: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get available padding third: Bad octet #0");
    return;
  }
  if (octets[1] not_eq 0x02) {
    test.fail("Input get available padding third: Bad octet #1");
    return;
  }
  test.pass("Input get available padding third: Passed");
}

void test_input_getn_invalid_third(TestState& test) {
  test_source src("AQI@");
  b64_input in(src);
  unsigned char octets[3];
  
  size_t count = in.getn(octets, 3);
  
  if (count not_eq 2) {
    test.fail("Input get invalid third: Bad count");
    return;
  }
  if (octets[0] not_eq 0x01) {
    test.fail("Input get invalid third: Bad octet #0");
    return;
  }
  if (octets[1] not_eq 0x02) {
    test.fail("Input get invalid third: Bad octet #1");
    return;
  }
  test.pass("Input get invalid third: Passed");
}

void test_input_skip_available(TestState& test) {
  test_source src("A QI\rDB\tAU\nG");
  b64_input in(src);
  
  size_t count = in.skip(7);
  
  if (count not_eq 6) {
    test.fail("Input skip available: Bad count");
    return;
  }
  test.pass("Input skip available: Passed");
}

void test_input_skip_requested(TestState& test) {
  test_source src("A QI\rDB\tAU\nG");
  b64_input in(src);
  
  size_t count = in.skip(5);
  
  if (count not_eq 5) {
    test.fail("Input skip requested: Bad count");
    return;
  }
  test.pass("Input get requested: Passed");
}

void test_input_skip_incomplete_first_one(TestState& test) {
  test_source src("");
  b64_input in(src);
  
  size_t count = in.skip(1);
  
  if (count not_eq 0) {
    test.fail("Input skip incomplete first one: Bad count");
    return;
  }
  test.pass("Input skip incomplete first one: Passed");
}

void test_input_skip_invalid_first_one(TestState& test) {
  test_source src("=");
  b64_input in(src);
  
  size_t count = in.skip(1);
  
  if (count not_eq 0) {
    test.fail("Input skip invalid first one: Bad count");
    return;
  }
  test.pass("Input skip invalid first one: Passed");
}

void test_input_skip_incomplete_first_two(TestState& test) {
  test_source src("A");
  b64_input in(src);
  
  size_t count = in.skip(1);
  
  if (count not_eq 0) {
    test.fail("Input skip incomplete first two: Bad count");
    return;
  }
  test.pass("Input skip incomplete first two: Passed");
}

void test_input_skip_invalid_first_two(TestState& test) {
  test_source src("A=");
  b64_input in(src);
  
  size_t count = in.skip(1);
  
  if (count not_eq 0) {
    test.fail("Input skip invalid first two: Bad count");
    return;
  }
  test.pass("Input skip invalid first two: Passed");
}

void test_input_skip_incomplete_second(TestState& test) {
  test_source src("AQ");
  b64_input in(src);
  
  size_t count = in.skip(2);
  
  if (count not_eq 1) {
    test.fail("Input skip incomplete second: Bad count");
    return;
  }
  test.pass("Input skip incomplete second: Passed");
}

void test_input_skip_padding_second(TestState& test) {
  test_source src("AQ==");
  b64_input in(src);
  
  size_t count = in.skip(2);
  
  if (count not_eq 1) {
    test.fail("Input skip available padding second: Bad count");
    return;
  }
  test.pass("Input skip available padding second: Passed");
}

void test_input_skip_invalid_second(TestState& test) {
  test_source src("AQ@");
  b64_input in(src);
  
  size_t count = in.skip(2);
  
  if (count not_eq 1) {
    test.fail("Input skip invalid second: Bad count");
    return;
  }
  test.pass("Input skip invalid second: Passed");
}

void test_input_skip_incomplete_third(TestState& test) {
  test_source src("AQI");
  b64_input in(src);
  
  size_t count = in.skip(3);
  
  if (count not_eq 2) {
    test.fail("Input skip incomplete third: Bad count");
    return;
  }
  test.pass("Input skip incomplete third: Passed");
}

void test_input_skip_padding_third(TestState& test) {
  test_source src("AQI=");
  b64_input in(src);
  
  size_t count = in.skip(3);
  
  if (count not_eq 2) {
    test.fail("Input skip available padding third: Bad count");
    return;
  }
  test.pass("Input skip available padding third: Passed");
}

void test_input_skip_invalid_third(TestState& test) {
  test_source src("AQI@");
  b64_input in(src);
  
  size_t count = in.skip(3);
  
  if (count not_eq 2) {
    test.fail("Input skip invalid third: Bad count");
    return;
  }
  test.pass("Input skip invalid third: Passed");
}

void test_output_putn_available(TestState& test) {
  string str;
  test_sink sink(str, 8);
  b64_output out(sink, 2, "\t");
  unsigned char const octets[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
  
  size_t count = out.putn(octets, 6);
  
  if (count not_eq 5) {
    test.fail("Output put available: Bad count");
    return;
  }
  if (str not_eq "AQ\tID\tBA") { // Check unable to write line separator
    test.fail("Output put available: Wrong string");
  }
  test.pass("Output put available: Passed");
}

void test_output_putn_requested(TestState& test) {
  string str;
  test_sink sink(str);
  b64_output out(sink, 2, "\t");
  unsigned char const octets[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
  
  size_t count = out.putn(octets, 6);
  
  if (count not_eq 6) {
    test.fail("Output put requested: Bad count");
    return;
  }
  if (str not_eq "AQ\tID\tBA\tUG") {
    test.fail("Output put requested: Wrong string");
  }
  test.pass("Output put requested: Passed");
}

void test_output_putn_incomplete_first(TestState& test) {
  string str;
  test_sink sink(str, 0);
  b64_output out(sink);
  unsigned char const octets[] = { 0x01, 0x02, 0x03 };
  
  size_t count = out.putn(octets, 3);
  
  if (count not_eq 0) {
    test.fail("Output put incomplete first: Bad count");
    return;
  }
  if (str not_eq "") {
    test.fail("Output put incomplete first: Wrong string");
  }
  test.pass("Output put incomplete first: Passed");
}

void test_output_putn_incomplete_second(TestState& test) {
  string str;
  test_sink sink(str, 1);
  b64_output out(sink);
  unsigned char const octets[] = { 0x01, 0x02, 0x03 };
  
  size_t count = out.putn(octets, 3);
  
  if (count not_eq 1) {
    test.fail("Output put incomplete second: Bad count");
    return;
  }
  if (str not_eq "A") {
    test.fail("Output put incomplete second: Wrong string");
  }
  test.pass("Output put incomplete second: Passed");
}

void test_output_putn_incomplete_third_one(TestState& test) {
  string str;
  test_sink sink(str, 2);
  b64_output out(sink);
  unsigned char const octets[] = { 0x01, 0x02, 0x03 };
  
  size_t count = out.putn(octets, 3);
  
  if (count not_eq 2) {
    test.fail("Output put incomplete third one: Bad count");
    return;
  }
  if (str not_eq "AQ") {
    test.fail("Output put incomplete third one: Wrong string");
  }
  test.pass("Output put incomplete third one: Passed");
}

void test_output_putn_incomplete_third_two(TestState& test) {
  string str;
  test_sink sink(str, 3);
  b64_output out(sink);
  unsigned char const octets[] = { 0x01, 0x02, 0x03 };
  
  size_t count = out.putn(octets, 3);
  
  if (count not_eq 2) {
    test.fail("Output put incomplete third two: Bad count");
    return;
  }
  if (str not_eq "AQI") {
    test.fail("Output put incomplete third two: Wrong string");
  }
  test.pass("Output put incomplete third two: Passed");
}

void acceptance_test_output_conclude_first(TestState& test) {
  string str;
  test_sink sink(str);
  b64_output out(sink);
  unsigned char const octets[] = { 0x01, 0x02, 0x03 };
  
  out.conclude();
  out.putn(octets, 1);
  
  if (str not_eq "A") {
    test.fail("Output conclude first: Wrong string");
  }
  test.pass("Output conclude first: Passed");
}

void acceptance_test_output_conclude_second(TestState& test) {
  string str;
  test_sink sink(str);
  b64_output out(sink);
  unsigned char const octets[] = { 0x01, 0x02, 0x03 };
  
  out.putn(octets, 1);
  out.conclude();
  out.putn(octets, 1);
  
  if (str not_eq "AQ==A") {
    test.fail("Output conclude second: Wrong string");
  }
  test.pass("Output conclude second: Passed");
}

void acceptance_test_output_conclude_third(TestState& test) {
  string str;
  test_sink sink(str);
  b64_output out(sink);
  unsigned char const octets[] = { 0x01, 0x02, 0x03 };
  
  out.putn(octets, 2);
  out.conclude();
  out.putn(octets, 1);
  
  if (str not_eq "AQI=A") {
    test.fail("Output conclude third: Wrong string");
  }
  test.pass("Output conclude third: Passed");
}

int main() {
  TestState test;
  try {
    test_input_getn_available(test);
    test_input_getn_requested(test);
    test_input_getn_incomplete_first_one(test);
    test_input_getn_invalid_first_one(test);
    test_input_getn_incomplete_first_two(test);
    test_input_getn_invalid_first_two(test);
    test_input_getn_incomplete_second(test);
    test_input_getn_padding_second(test);
    test_input_getn_invalid_second(test);
    test_input_getn_incomplete_third(test);
    test_input_getn_padding_third(test);
    test_input_getn_invalid_third(test);
    
    test_input_skip_available(test);
    test_input_skip_requested(test);
    test_input_skip_incomplete_first_one(test);
    test_input_skip_invalid_first_one(test);
    test_input_skip_incomplete_first_two(test);
    test_input_skip_invalid_first_two(test);
    test_input_skip_incomplete_second(test);
    test_input_skip_padding_second(test);
    test_input_skip_invalid_second(test);
    test_input_skip_incomplete_third(test);
    test_input_skip_padding_third(test);
    test_input_skip_invalid_third(test);
    
    test_output_putn_available(test);
    test_output_putn_requested(test);
    test_output_putn_incomplete_first(test);
    test_output_putn_incomplete_second(test);
    test_output_putn_incomplete_third_one(test);
    test_output_putn_incomplete_third_two(test);
    
    acceptance_test_output_conclude_first(test);
    acceptance_test_output_conclude_second(test);
    acceptance_test_output_conclude_third(test);
  }
  catch (...) {
    test.fail("Something went wrong...");
  }
  return EXIT_SUCCESS;
}
