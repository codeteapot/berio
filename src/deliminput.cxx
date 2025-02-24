#include <algorithm>
#include <limits>

#include <berio/deliminput.h>

using namespace ber;
using namespace std;

delimited_input::sentry::sentry(delimited_input& in, size_t len, bool delim) noexcept:
__in(in),
__nr(delim ? __in.narrow(len) : narrow_result{ 0, false }) {}

delimited_input::sentry::~sentry() {
  if (*this) __in.restore(__nr.prev_end);
}

delimited_input::sentry::operator bool () const noexcept {
  return __nr.applied;
}

delimited_input::skippable::~skippable() {}

delimited_input::delimited_input(skippable& in) noexcept:
__in(in),
__off(0),
__end(numeric_limits<std::size_t>::max()) {}

#ifdef LIBBERIO_UNIT_TESTING_ENABLED
delimited_input::delimited_input(skippable& in, size_t off, size_t end) noexcept:
__in(in),
__off(off),
__end(end) {}
#endif

delimited_input::narrow_result delimited_input::narrow(size_t len) noexcept {
  if (len > __remaining())
    return { 0, false };
  size_t const prev_end = __end;
  __end = __off + len;
  return { prev_end, true };
}

void delimited_input::restore(size_t prev_end) {
  __off += __in.skip(__remaining()); 
  __end = prev_end;
}

size_t delimited_input::getn(unsigned char* optr, size_t len) {
  size_t n = __in.getn(optr, min(len, __remaining()));
  return __off += n, n;
}

#ifdef LIBBERIO_UNIT_TESTING_ENABLED
size_t delimited_input::current_off() const noexcept {
  return __off;
}

size_t delimited_input::current_end() const noexcept {
  return __end;
}
#endif

size_t delimited_input::__remaining() const {
  return __end - __off;
}
