#include <algorithm>

#include <berio/deliminput.h>

using namespace ber;
using namespace std;

constexpr delimited_input::segment __indefinite_delimiter = { 0, false };

delimited_input::delimit_sentry::delimit_sentry(delimited_input& in, bool definite, size_t length):
__in(in),
__delimiter(definite ? __in.delimit(length) : __indefinite_delimiter) {}

delimited_input::delimit_sentry::~delimit_sentry() noexcept {
  __in.delimit(__delimiter);
}

delimited_input::delimited_input(octet_input& in):
__in(in),
__off(0),
__delimiter(__indefinite_delimiter) {}

bool delimited_input::definite() const {
  return __delimiter.definite;
}

delimited_input::segment delimited_input::delimit(size_t length) {
  segment const delimiter = __delimiter;
  size_t const end = __off + length;
  __delimiter.end = __delimiter.definite ? min(__delimiter.end, end) : end;
  __delimiter.definite = true;
  return delimiter;
}

void delimited_input::delimit(segment const& delimiter) {
  if (__delimiter.definite)
    __off += __in.skip(__remaining()); 
  __delimiter = delimiter;
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
  return __delimiter.definite ? min(len, __remaining()) : len;
}

size_t delimited_input::__remaining() const {
  return __off < __delimiter.end ? __delimiter.end - __off : 0;
}
