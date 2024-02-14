#include <algorithm>

#include <berio/deliminput.h>

using namespace ber;
using namespace std;

delimited_input::delimit_sentry::delimit_sentry(delimited_input& in, tag_length const& len):
__in(in),
__delim(__in.delimit(len)) {}

delimited_input::delimit_sentry::~delimit_sentry() noexcept {
  __in.delimit(__delim);
}

delimited_input::delimited_input(octet_input& in):
__in(in),
__off(0),
__delim({0, false}) {}

delimited_input::segment delimited_input::delimit(tag_length const& tlen) {
  segment const delim = __delim; 
  if (tlen.definite) {
    size_t const end = __off + tlen.value;
    __delim.end = __delim.definite ? min(__delim.end, end) : end;
    __delim.definite = true;
  }
  return delim;
}

void delimited_input::delimit(segment const& delim) {
  if (__delim.definite)
    __off += __in.skip(__remaining()); 
  __delim = delim;
}

size_t delimited_input::get(unsigned char* optr, size_t len) {
  size_t avail = __in.get(optr, __available(len));
  return __off += avail, avail;
}

size_t delimited_input::skip(size_t len) {
  size_t skipped = __in.skip(__available(len));
  return __off += skipped, skipped;
}

size_t delimited_input::__available(size_t len) const {
  return __delim.definite ? min(len, __remaining()) : len;
}

size_t delimited_input::__remaining() const {
  return __off < __delim.end ? __delim.end - __off : 0;
}
