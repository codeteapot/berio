#include <berio/b64istream.h>
#include <berio/b64ostream.h>

#include <dejagnu.h>

using namespace ber;
using namespace std;

void test_source_traits_peek_available(TestState& test) {
  stringstream ss;
  ss << "12";
  
  char c = 3;
  bool avail = b64_source_traits<stringstream>::peek(ss, c);
  
  if (not avail) {
    test.fail("Source traits peek available: Not available");
    return;
  }
  if (c not_eq '1') {
    test.fail("Source traits peek available: Bad character");
    return;
  }
  ss >> c;
  if (c not_eq '1') {
    test.fail("Source traits peek available: Character lost");
    return;
  }
  test.pass("Source traits peek available: Passed");
}

void test_source_traits_peek_unavailable(TestState& test) {
  stringstream ss;
  
  char c = '3';
  bool avail = b64_source_traits<stringstream>::peek(ss, c);
  
  if (avail) {
    test.fail("Source traits peek unavailable: Available");
    return;
  }
  if (c not_eq '3') {
    test.fail("Source traits peek unavailable: Character changed");
    return;
  }
  test.pass("Source traits peek unavailable: Passed");
}

void test_source_traits_ignore(TestState& test) {
  stringstream ss;
  ss << "12";
  
  b64_source_traits<stringstream>::ignore(ss);
  
  char c = '3';
  ss >> c;
  if (c not_eq '2') {
    test.fail("Source traits ignore: Not ignored");
    return;
  }
  test.pass("Source traits ignore: Passed");
}

void test_sink_traits_put_available(TestState& test) {
  stringstream ss;
  
  bool avail = b64_sink_traits<stringstream>::put(ss, '1');
  
  if (not avail) {
    test.fail("Sink traits put available: Not available");
    return;
  }
  char c = '2';
  ss >> c;
  if (c not_eq '1') {
    test.fail("Sink traits put available: Bad character");
    return;
  }
  test.pass("Sink traits put available: Passed");
}

void test_sink_traits_put_unavailable(TestState& test) {
  stringstream ss;
  ss.setstate(ios_base::eofbit);
  
  bool avail = b64_sink_traits<stringstream>::put(ss, '1');
  
  if (avail) {
    test.fail("Sink traits put unavailable: Available");
    return;
  }
  char c = '2';
  ss >> c;
  if (c not_eq '2') {
    test.fail("Sink traits put unavailable: Character inserted");
    return;
  }
  test.pass("Sink traits put unavailable: Passed");
}

int main() {
  TestState test;
  try {
    test_source_traits_peek_available(test);
    test_source_traits_peek_unavailable(test);
    test_source_traits_ignore(test);
    
    test_sink_traits_put_available(test);
    test_sink_traits_put_unavailable(test);
  }
  catch (...) {
    test.fail("Something went wrong...");
  }
  return EXIT_SUCCESS;
}
