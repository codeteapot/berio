#include <algorithm>

#include <berio/deliminput.h>

using namespace ber;
using namespace std;

delimited_input::sentry::sentry(delimited_input& in, size_t len) noexcept:
__in(in),
__prev_end(__in.narrow(len)) {}

delimited_input::sentry::~sentry() noexcept {
  __in.restore(__prev_end);
}

delimited_input::skippable::~skippable() {}

delimited_input::delimited_input(skippable& in) noexcept:
__in(in),
__off(0),
__end(max_length) {}

size_t delimited_input::narrow(size_t len) noexcept {
  size_t const prev_end = __end;
  __end = min(__end, __off + min(len, __remaining()));
  return prev_end;
}

void delimited_input::restore(size_t prev_end) {
  __off += __in.skip(__remaining()); 
  __end = prev_end;
}

size_t delimited_input::getn(unsigned char* optr, size_t len) {
  size_t avail = __in.getn(optr, min(len, __remaining()));
  return __off += avail, avail;
}

size_t delimited_input::__remaining() const {
  return __end - __off;
}
