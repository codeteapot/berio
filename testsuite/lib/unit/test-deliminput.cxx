#include <cstdlib>

#include <berio/deliminput.h>

#include <arrayio.h>
#include <tstub.h>
#include <dejagnu.h>

using namespace std;
using namespace ber;

template<typename NarrowStub, typename RestoreStub>
class test_delimited_input : public delimited_input {

  public:
  
  test_delimited_input(NarrowStub& narrow_stub, RestoreStub& restore_stub) noexcept:
  delimited_input(__empty_in),
  __narrow_stub(narrow_stub),
  __restore_stub(restore_stub) {}
  
  narrow_result narrow(size_t len) noexcept {
    return __narrow_stub.call(len);
  }
  
  void restore(size_t prev_end) {
    return __restore_stub.call(prev_end);
  }
  
  private:
  
  class empty_skippable : public delimited_input::skippable {
  
    public:
    
    size_t getn(unsigned char* optr, size_t len) override {
      return 0;
    }
    
    size_t skip(size_t len) override {
      return 0;
    }
  };
  
  empty_skippable __empty_in;
  
  NarrowStub& __narrow_stub;
  RestoreStub& __restore_stub;
};

template<typename GetnStub, typename SkipStub>
class test_skippable : public delimited_input::skippable {

  public:
  
  test_skippable(GetnStub& getn_stub, SkipStub& skip_stub) noexcept:
  __getn_stub(getn_stub),
  __skip_stub(skip_stub) {}
  
  size_t getn(unsigned char* optr, size_t len) override {
    return __getn_stub.call(optr, len);
  }
  
  size_t skip(size_t len) override {
    return __skip_stub.call(len);
  }
  
  private:
  
  GetnStub& __getn_stub;
  SkipStub& __skip_stub;
};

void test_narrow_not_applied(TestState& test) {
  constexpr size_t off = 4;
  constexpr size_t end = 5;
  constexpr size_t len = 2;
  
  test::stub getn_stub{
    test::when(
      test::is_called_with(test::matchers::any<unsigned char*>(), test::matchers::any<size_t>()),
      test::then_return(0))
  };
  test::stub skip_stub{
    test::when(
      test::is_called_with(test::matchers::any<size_t>()),
      test::then_return(0))
  };
  test_skippable in(getn_stub, skip_stub);
  delimited_input delim_in(in, off, end);
  
  delimited_input::narrow_result nr = delim_in.narrow(len);
  if (nr.applied) {
    test.fail("Narrow not applied: Bad applied result value");
    return;
  }
  if (delim_in.current_off() not_eq off) {
    test.fail("Narrow not applied: Bad current offset value");
    return;
  }
  if (delim_in.current_end() not_eq end) {
    test.fail("Narrow not applied: Bad current end value");
    return;
  }
  
  test.pass("Narrow not applied: Passed");
}

void test_narrow_applied(TestState& test) {
  constexpr size_t off = 1;
  constexpr size_t end = 5;
  constexpr size_t len = 2;
  constexpr size_t new_end = 3;
  
  test::stub getn_stub{
    test::when(
      test::is_called_with(test::matchers::any<unsigned char*>(), test::matchers::any<size_t>()),
      test::then_return(0))
  };
  test::stub skip_stub{
    test::when(
      test::is_called_with(test::matchers::any<size_t>()),
      test::then_return(0))
  };
  test_skippable in(getn_stub, skip_stub);
  delimited_input delim_in(in, off, end);
  
  delimited_input::narrow_result nr = delim_in.narrow(len);
  if (not nr.applied) {
    test.fail("Narrow applied: Bad applied result value");
    return;
  }	
  if (nr.prev_end not_eq end) {
    test.fail("Narrow applied: Bad previous end result value");
    return;
  }
  if (delim_in.current_off() not_eq off) {
    test.fail("Narrow applied: Bad current offset value");
    return;
  }
  if (delim_in.current_end() not_eq new_end) {
    test.fail("Narrow applied: Bad current end value");
    return;
  }
  test.pass("Narrow applied: Passed");
}

void test_restore(TestState& test) {
  constexpr size_t off = 2;
  constexpr size_t end = 6;
  constexpr size_t rem = 4;
  constexpr size_t prev_end = 7;
  constexpr size_t skipped = 3;
  constexpr size_t new_off = 5;
  
  test::stub getn_stub{
    test::when(
      test::is_called_with(test::matchers::any<unsigned char*>(), test::matchers::any<size_t>()),
      test::then_return(0))
  };
  test::stub skip_stub{
    test::when(
      test::is_called_with(rem),
      test::then_return(skipped))
  };
  test_skippable in(getn_stub, skip_stub);
  delimited_input delim_in(in, off, end);
  
  delim_in.restore(prev_end);
  if (not skip_stub.verify(rem)) { // Redundant
    test.fail("Restore: Bad skipping");
    return;
  }
  if (delim_in.current_off() not_eq new_off) {
    test.fail("Restore: Bad current offset value");
    return;
  }
  if (delim_in.current_end() not_eq prev_end) {
    test.fail("Restore: Bad current end value");
    return;
  }
  test.pass("Restore: Passed");
}

void test_getn_requested(TestState& test) {
  constexpr size_t end = 2;
  constexpr size_t len = 1;
  constexpr unsigned char octet_i0 = 0x0c;
  unsigned char octets[3];
  
  array_octet_input in(make_octet_array(octet_i0, 0xe1, 0xf1));  
  delimited_input delim_in(in, 0, end);
  
  size_t count = delim_in.getn(octets, len);
  
  if (count not_eq len) {
    test.fail("Get requested octets: Bad count");
    return;
  }
  if (octets[0] not_eq octet_i0) {
    test.fail("Get requested octets: Bad octet #0");
    return;
  }
  if (delim_in.current_off() not_eq len) {
    test.fail("Get requested octets: Bad current offset value");
    return;
  }
  if (delim_in.current_end() not_eq end) {
    test.fail("Get requested octets: Bad current end value");
    return;
  }
  test.pass("Get requested octets: Passed");
}

void test_getn_remaining(TestState& test) {
  constexpr size_t end = 1;
  constexpr size_t len = 2;
  constexpr unsigned char octet_i0 = 0x0c;
  unsigned char octets[3];
  
  array_octet_input in(make_octet_array(octet_i0, 0xe1, 0xf1));  
  delimited_input delim_in(in, 0, end);
  
  size_t count = delim_in.getn(octets, len);
  
  if (count not_eq end) {
    test.fail("Get remaining octets: Bad count");
    return;
  }
  if (octets[0] not_eq octet_i0) {
    test.fail("Get remaining octets: Bad octet #0");
    return;
  }
  if (delim_in.current_off() not_eq end) {
    test.fail("Get remaining octets: Bad current offset value");
    return;
  }
  if (delim_in.current_end() not_eq end) {
    test.fail("Get remaining octets: Bad current end value");
    return;
  }
  test.pass("Get remaining octets: Passed");
}

/*
 * NOTE Testing sentry boolean evaluation along with constructor and destructor as a unit
 */

void test_sentry_narrow_applied(TestState& test) {
  constexpr size_t len = 2;
  constexpr size_t prev_end = 5;
  
  test::stub narrow_stub{
    test::when(
      test::is_called_with(len),
      test::then_return(delimited_input::narrow_result{ prev_end, true }))
  };
  test::stub restore_stub{
    test::when(
      test::is_called_with(test::matchers::any<size_t>()),
      test::do_nothing())
  };
  test_delimited_input delim_in(narrow_stub, restore_stub);
  
  // Sentry scope
  {
    delimited_input::sentry delimit(delim_in, len, true);
    
    if (not narrow_stub.verify(len)) { // Redundant
      test.fail("Sentry narrow applied: Bad narrowing");
      return;
    }
    if (not delimit) {
      test.fail("Sentry narrow applied: Bad boolean evaluation");
      return;
    }
  }
  if (not restore_stub.verify(prev_end)) {
    test.fail("Sentry narrow applied: Bad restoring");
    return;
  }
  test.pass("Sentry narrow applied: Passed");
}

void test_sentry_narrow_not_applied(TestState& test) {
  constexpr size_t len = 2;
  
  test::stub narrow_stub{
    test::when(
      test::is_called_with(len),
      test::then_return(delimited_input::narrow_result{ 0, false }))
  };
  test::stub restore_stub{
    test::when(
      test::is_called_with(test::matchers::any<size_t>()),
      test::do_nothing())
  };
  test_delimited_input delim_in(narrow_stub, restore_stub);
  
  // Sentry scope
  {
    delimited_input::sentry delimit(delim_in, len, true);
    
    if (not narrow_stub.verify(len)) { // Redundant
      test.fail("Sentry narrow not applied: Bad narrowing");
      return;
    }
    if (delimit) {
      test.fail("Sentry narrow not applied: Bad boolean evaluation");
      return;
    }
  }
  if (restore_stub.verify(test::matchers::any<size_t>())) {
    test.fail("Sentry narrow not applied: Unwanted restoring");
    return;
  }
  test.pass("Sentry narrow not applied: Passed");
}

void test_sentry_not_delim(TestState& test) {
  test::stub narrow_stub{
    test::when(
      test::is_called_with(test::matchers::any<size_t>()),
      test::then_return(delimited_input::narrow_result{ 0, true }))
  };
  test::stub restore_stub{
    test::when(
      test::is_called_with(test::matchers::any<size_t>()),
      test::do_nothing())
  };
  test_delimited_input delim_in(narrow_stub, restore_stub);
  
  // Sentry scope
  {
    delimited_input::sentry delimit(delim_in, 2, false);
    
    if (narrow_stub.verify(test::matchers::any<size_t>())) { // Redundant
      test.fail("Sentry not delim: Unwanted narrowing");
      return;
    }
    if (delimit) {
      test.fail("Sentry not delim: Bad boolean evaluation");
      return;
    }
  }
  if (restore_stub.verify(test::matchers::any<size_t>())) {
    test.fail("Sentry not delim: Unwanted restoring");
    return;
  }
  test.pass("Sentry not delim: Passed");
}

int main() {
  TestState test;
  try {
    test_narrow_not_applied(test);
    test_narrow_applied(test);
    test_restore(test);
    test_getn_requested(test);
    test_getn_remaining(test);
    test_sentry_narrow_applied(test);
    test_sentry_narrow_not_applied(test);
    test_sentry_not_delim(test);
  }
  catch (...) {
    test.fail("Something went wrong...");
  }
  return EXIT_SUCCESS;
}
