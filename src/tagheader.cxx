#include <berio/tag.h>

using namespace ber;

bool tag_header::operator == (tag_header const& th) const {
  return tclass == th.tclass and number == th.number and shape == th.shape and length == th.length;
}
