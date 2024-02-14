#include <berio/b64.h>

using namespace ber;

constexpr unsigned char const invalid_sextet = 0xc0;

// TODO Use memory tables through compiler to do sextet/alphabet translation

unsigned char alphabet_sextet(char c) {
  if (c >= 'A' and c <= 'Z')
    return c - 'A';
  if (c >= 'a' and c <= 'z')
    return c - 'a' + 26;
  if (c >= '0' and c <= '9')
    return c - '0' + 52;
  if (c == '+')
    return 62;
  if (c == '/')
    return 63;
  return invalid_sextet;
}

char sextet_alphabet(unsigned char s) {
  if (s >= 0 and s <= 25)
    return s + 'A';
  if (s >= 26 and s <= 51)
    return s - 26 + 'a';
  if (s >= 52 and s <= 61)
    return s - 52 + '0';
  if (s == 62)
    return '+';
  return '/'; // PRE s < 64, so s = 63 here
}

b64_input::b64_input(source& src) noexcept:
__src(src),
__last_s(0),
__get_fn(&b64_input::__get_first) {}

size_t b64_input::get(unsigned char* optr, size_t len) {
  size_t actual_len = 0;
  while (actual_len < len and (this->*__get_fn)(optr + actual_len))
    ++actual_len;
  return actual_len;
}

size_t b64_input::skip(size_t len) {
  unsigned char null;
  size_t actual_len = 0;
  while (actual_len < len and (this->*__get_fn)(&null))
    ++actual_len;
  return actual_len;
}

void b64_input::__to_first() {
  __last_s = 0; // Not strictly needed
  __get_fn = &b64_input::__get_first;
}

void b64_input::__to_second(unsigned char last_s) {
  __last_s = last_s;
  __get_fn = &b64_input::__get_second;
}

void b64_input::__to_third(unsigned char last_s) {
  __last_s = last_s;
  __get_fn = &b64_input::__get_third;
}

bool b64_input::__get_first(unsigned char* optr) {
  char c;
  if (not __src.peek(c))
    return false;
  unsigned char const s1 = alphabet_sextet(c);
  if (s1 == invalid_sextet) // Including '='
    return false;
  __src.ignore();
  if (not __src.peek(c))
    return false;
  unsigned char const s2 = alphabet_sextet(c);
  if (s2 == invalid_sextet) // Including '='
    return false;
  __src.ignore();
  *optr = s1 << 2 bitor s2 >> 4;
  __to_second(s2);
  return true;
}

bool b64_input::__get_second(unsigned char* optr) {
  char c;
  if (not __src.peek(c))
    return false;
  if (c == '=') {
    __src.ignore();
    __src.ignore(); // Assumes that next character is '='
    return false;
  }
  unsigned char const s = alphabet_sextet(c);
  if (s == invalid_sextet)
    return false;
  __src.ignore();
  *optr = __last_s << 4 bitor s >> 2;
  __to_third(s);
  return true;
}

bool b64_input::__get_third(unsigned char* optr) {
  char c;
  if (not __src.peek(c))
    return false;
  if (c == '=') {
    __src.ignore();
    return false;
  }
  unsigned char const s = alphabet_sextet(c);
  if (s == invalid_sextet) // Including '='
    return false;
  __src.ignore();
  *optr = __last_s << 6 bitor s;
  __to_first();
  return true;
}

b64_output::b64_output(target& tgt) noexcept:
__tgt(tgt),
__last_o(0), // Not actually needed
__put_fn(&b64_output::__put_first),
__conclude_fn(&b64_output::__conclude_first) {}

size_t b64_output::put(unsigned char const* optr, size_t len) {
  size_t actual_len = 0;
  while (actual_len < len and (this->*__put_fn)(optr + actual_len))
    ++actual_len;
  return actual_len;
}

void b64_output::conclude() {
  (this->*__conclude_fn)();
}

void b64_output::__to_first() {
  __last_o = 0; // Not strictly needed
  __put_fn = &b64_output::__put_first;
  __conclude_fn = &b64_output::__conclude_first;
}

void b64_output::__to_second(unsigned char last_o) {
  __last_o = last_o;
  __put_fn = &b64_output::__put_second;
  __conclude_fn = &b64_output::__conclude_second;
}

void b64_output::__to_third(unsigned char last_o) {
  __last_o = last_o;
  __put_fn = &b64_output::__put_third;
  __conclude_fn = &b64_output::__conclude_third;
}

bool b64_output::__put_first(unsigned char const* optr) {
  unsigned char const o = *optr;
  unsigned char const s = sextet_alphabet(o >> 2);
  if (__tgt.put(s)) {
    __to_second(o);
    return true;
  }
  return false;
}

bool b64_output::__put_second(unsigned char const* optr) {
  unsigned char const o = *optr;
  unsigned char const s = sextet_alphabet((__last_o bitand 0x03) << 4 bitor o >> 4);
  if (__tgt.put(s)) {
    __to_third(o);
    return true;
  }
  return false;
}

bool b64_output::__put_third(unsigned char const* optr) {
  unsigned char const o = *optr;
  unsigned char const s1 = sextet_alphabet((__last_o bitand 0x0f) << 2 bitor o  >> 6);
  unsigned char const s2 = sextet_alphabet(0 bitand 0x3f);
  if (__tgt.put(s1) and __tgt.put(s2)) {
    __to_first();
    return true;
  }
  return false;
}

void b64_output::__conclude_first() {
  // Nothing to do
}

void b64_output::__conclude_second() {
  unsigned char const s = sextet_alphabet((__last_o bitand 0x03) << 4);
  if (__tgt.put(s) and __tgt.put('=') and __tgt.put('='))
    __to_first();
}

void b64_output::__conclude_third() {
  unsigned char const s = sextet_alphabet((__last_o bitand 0x0f) << 2);
  if (__tgt.put(s) and __tgt.put('='))
    __to_first();
}
