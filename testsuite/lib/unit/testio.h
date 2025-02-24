#ifndef UNIT_TESTS_TESTIO_H
#define UNIT_TESTS_TESTIO_H

#include <array>
#include <cmath>

#include <berio/io.h>

template<std::size_t Size>
using octet_array = std::array<unsigned char, Size>;

template<typename T>
concept octet_array_like = requires(T a) {
  { a.size() } -> std::convertible_to<std::size_t>;
  requires std::same_as<typename T::value_type, unsigned char>;
};

consteval auto make_octet_array() {
  return octet_array<0>{};
}

template<typename T, typename... Types>
consteval auto make_octet_array(T o, Types... args)
requires std::integral<T> {
  auto a = make_octet_array(args...);
  octet_array<a.size() + 1> r{};
  r[0] = static_cast<unsigned char>(o & 0xff);
  std::copy(std::begin(a), std::end(a), std::begin(r) + 1);
  return r;
}

template<typename F, typename... Types>
consteval auto make_octet_array(F&& fn, Types... args)
requires std::invocable<F> and octet_array_like<std::remove_cvref_t<decltype(fn())>> {
  auto fa = fn();
  auto a = make_octet_array(args...);
  octet_array<fa.size() + a.size()> r{};
  std::copy(std::begin(fa), std::end(fa), std::begin(r));
  std::copy(std::begin(a), std::end(a), std::begin(r) + fa.size());
  return r;
}

template<std::size_t Size>
class array_octet_input : public ber::octet_input {

  public:
  
  constexpr array_octet_input(octet_array<Size> arr):
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
  
  octet_array<Size> __arr;
  std::size_t __pos;
};

template<std::size_t Size>
class array_octet_output : public ber::octet_output {

  public:
  
  array_octet_output(octet_array<Size>& arr):
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
  
  octet_array<Size>& __arr;
  std::size_t __pos;
};

#endif // UNIT_TESTS_TESTIO_H
