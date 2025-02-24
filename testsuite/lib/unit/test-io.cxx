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
  
  private:
  
  VdestrStub& __vdestr_stub;
  GetnStub& __getn_stub;
};

template<typename VdestrStub, typename PutnStub>
class test_octet_output : public octet_output {

  public:
  
  test_octet_output(VdestrStub& vdestr_stub, PutnStub& putn_stub):
  __vdestr_stub(vdestr_stub),
  __putn_stub(putn_stub) {}
  
  ~test_octet_output() {
    __vdestr_stub.call();
  }
  
  size_t putn(unsigned char const* optr, size_t len) {
    return __putn_stub.call(optr, len);
  }
  
  void flush() {}
  
  private:
  
  VdestrStub& __vdestr_stub;
  PutnStub& __putn_stub;
};

void test_input_virtual_destructor(TestState& test) {
  test::stub vdestr_stub{
    test::when(
      test::is_called_with(),
      test::do_nothing())
  };
  test::stub getn_stub{
    test::when(
      test::is_called_with(test::matchers::any<unsigned char*>(), test::matchers::any<size_t>()),
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
      test::is_called_with(),
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

void test_output_virtual_destructor(TestState& test) {
  test::stub vdestr_stub{
    test::when(
      test::is_called_with(),
      test::do_nothing())
  };
  test::stub putn_stub{
    test::when(
      test::is_called_with(
        test::matchers::any<unsigned char const*>(),
        test::matchers::any<size_t>()),
      test::then_return(0))
  };
  octet_output* out_ptr = new test_octet_output(vdestr_stub, putn_stub);
  
  delete out_ptr;
  
  if (not vdestr_stub.verify()) {
    test.fail("Output virtual destructor: Not called");
    return;
  }
  test.pass("Output virtual destructor: Passed");
}

void test_output_put_one(TestState& test) {
  unsigned char const o = 0x00;
  test::stub vdestr_stub{
    test::when(
      test::is_called_with(),
      test::do_nothing())
  };
  test::stub putn_stub{
    test::when(
      test::is_called_with(test::matchers::that([](unsigned char const* optr) {
        return optr[0] == o;
      }), 1),
      test::then_return(1))
  };
  test_octet_output out(vdestr_stub, putn_stub);
  
  bool b = out.put(o);
  
  if (not b) {
    test.fail("Output put one: Bad result");
    return;
  }
  test.pass("Output put one: Passed");
}

int main() {
  TestState test;
  try {
    test_input_virtual_destructor(test);
    test_input_get_one(test);
    
    test_output_virtual_destructor(test);
    test_output_put_one(test);
  }
  catch (...) {
    test.fail("Something went wrong...");
  }
  return EXIT_SUCCESS;
}
