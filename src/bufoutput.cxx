#include <cmath>
#include <cstring>

#include <berio/bufoutput.h>

using namespace ber;
using namespace std;

buffered_output::buffered_output(octet_output& out):
__out(out),
__first(new buffer_array<32>()),
__last(__first),
__last_off(0),
__len(0),
__allocate_fn(&buffered_output::__allocate_256b) {}

buffered_output::~buffered_output() {
  buffer_node* n = __first;
  while (n not_eq nullptr) {
    buffer_node const* nd = n;
    n = n->next();
    delete nd;
  }
}

size_t buffered_output::length() const { return __len; }

size_t buffered_output::put(unsigned char const* optr, size_t len) {
  size_t count = __last->copy(optr, __last_off, len);
  size_t accum = count;
  while (accum < len) {
    __allocate();
    __last_off = 0;
    count = __last->copy(optr + accum, __last_off, len - accum);
    accum += count;
  }
  __last_off += count;
  __len += len;
  return len;
}

void buffered_output::flush() {
  for (buffer_node const* n = __first; n not_eq __last; n = n->next()) {
    n->dump(__out);
  }
  __last->dump(__out, __last_off);
  __last = __first;
  __last_off = 0;
}

template <size_t array_size>
void buffered_output::__allocate(void (buffered_output::*next_alloc_fn)()) {
  __last = __last->next() == nullptr
    ? __last->next(new buffer_array<array_size>())
    : __last->next();
  __allocate_fn = next_alloc_fn;
}

void buffered_output::__allocate() {
  (this->*__allocate_fn)();
}

void buffered_output::__allocate_256b() {
  __allocate<256>(&buffered_output::__allocate_1kb);
}

void buffered_output::__allocate_1kb() {
  __allocate<1024>(&buffered_output::__allocate_4kb);
}

void buffered_output::__allocate_4kb() {
  __allocate<4094>(&buffered_output::__allocate_4kb);
}

buffered_output::buffer_node::buffer_node():
__next(nullptr) {}

buffered_output::buffer_node* buffered_output::buffer_node::next() const {
  return __next;
}

buffered_output::buffer_node* buffered_output::buffer_node::next(buffer_node* n) {
  return __next = n;
}

template <size_t array_size>
void buffered_output::buffer_array<array_size>::dump(octet_output& out, size_t len) const {
  out.put(__array, len);
}

template <size_t array_size>
void buffered_output::buffer_array<array_size>::dump(octet_output& out) const {
  dump(out, array_size);
}

template <size_t array_size>
size_t buffered_output::buffer_array<array_size>::copy(
    unsigned char const* optr,
    size_t off,
    size_t len) {
  const size_t count = min(array_size - off, len);
  memcpy(__array + off, optr, count);
  return count;
}
