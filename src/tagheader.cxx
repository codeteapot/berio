#include <berio/tag.h>

using namespace ber;

bool ber::operator == (tag_header const& th1, tag_header const& th2) {
  return th1.tclass == th2.tclass and
      th1.number == th1.number and
      th1.shape == th2.shape and
      th1.length == th2.length;
}
