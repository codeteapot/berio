#include <cstdlib>
#include <functional>

#include <berio/io.h>

#include <dejagnu.h>
#include <tstub.h>

using namespace ber;
using namespace std;

template<typename VdestrStub, typename GetnStub>
class test_octet_input : public octet_input {

  public:
  
  test_octet_input(VdestrStub& vdestr_stub, GetnStub& getn_stub):
  __vdestr_stub(vdestr_stub),
  __getn_stub(getn_stub) {}
  
  ~test_octet_input() {
    __vdestr_stub.call();
  }
  
  size_t getn(unsigned char* optr, size_t len) override {
    return __getn_stub.call(optr, len);
  }
  
  size_t skip(size_t) override {
    return 0;
  }
  
  private:
  
  VdestrStub& __vdestr_stub;
  GetnStub& __getn_stub;
};

void test_input_virtual_destructor(TestState& test) {
  test::stub vdestr_stub{
    test::when(
      test::is_called(),
      test::do_nothing())
  };
  test::stub getn_stub{
    test::when(
      test::is_called<unsigned char*, size_t>(),
      test::then_return(0))
  };
  octet_input* in_ptr = new test_octet_input(vdestr_stub, getn_stub);
  
  delete in_ptr;
  
  if (not vdestr_stub.verify()) {
    test.fail("Input virtual destructor: Not called");
    return;
  }
  test.pass("Input virtual destructor: Passed");
}

void test_input_get_one(TestState& test) {
  unsigned char octets[] = { 0x00 };
  test::stub vdestr_stub{
    test::when(
      test::is_called(),
      test::do_nothing())
  };
  test::stub getn_stub{
    test::when(
      test::is_called_with(octets, 1),
      test::then_return(1))
  };
  test_octet_input in(vdestr_stub, getn_stub);
  
  bool b = in.get(octets[0]);
  
  if (not b) {
    test.fail("Input get one: Bad result");
    return;
  }
  test.pass("Input get one: Passed");
}

int main() {
  TestState test;
  try {
    test_input_virtual_destructor(test);
    test_input_get_one(test);
  }
  catch (...) {
    test.fail("Something went wrong...");
  }
  return EXIT_SUCCESS;
}
