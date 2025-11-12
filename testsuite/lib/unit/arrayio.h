#ifndef UNIT_TESTS_ARRAYIO_H
#define UNIT_TESTS_ARRAYIO_H

#include <cmath>

#include <berio/io.h>

template<std::size_t Size = 0>
class array_octet_input: public ber::octet_input {

  public:
  
  array_octet_input(unsigned char const(&arr)[Size]):
  __arr(arr),
  __pos(0) {}
  
  ~array_octet_input() {}
  
  bool get(unsigned char& o) {
    if (__pos < Size) {
      o = __arr[__pos++];
      return true;
    }
    return false;
  }
  
  std::size_t get(unsigned char* optr, std::size_t len) {
    std::size_t count = 0;
    while (count < len and __pos < Size)
      optr[count++] = __arr[__pos++];
    return count;
  }
  
  std::size_t skip(std::size_t len) {
    std::size_t const count = std::min(Size - __pos, len);
    __pos += count;
    return count;
  }
  
  private:
  
  unsigned char const(&__arr)[Size];
  std::size_t __pos;
};

template<std::size_t Size>
class array_octet_output: public ber::octet_output {

  public:
  
  array_octet_output(unsigned char(&arr)[Size]):
  __arr(arr),
  __pos(0) {}
  
  std::size_t getpos() const {
    return __pos;
  }
  
  bool put(unsigned char o) {
    if (__pos < Size) {
      __arr[__pos++] = o;
      return true;
    }
    return false;
  }
  
  std::size_t put(unsigned char const* optr, std::size_t len) {
    std::size_t count = 0;
    while (count < len and __pos < Size)
      __arr[__pos++] = optr[count++];
    return count;
  }
  
  void flush() {}
  
  private:
  
  unsigned char(&__arr)[Size];
  std::size_t __pos;
};

#endif // UNIT_TESTS_ARRAYIO_H
