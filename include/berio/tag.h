#ifndef BERIO_TAG_H
#define BERIO_TAG_H

#include <berio/io.h>

namespace ber {

enum tag_class {

  tc_universal = 0,
  tc_application = 1,
  tc_context_specific = 2,
  tc_private = 3
};

typedef unsigned int tag_number;

enum tag_shape {

  ts_primitive,
  ts_constructed_definite,
  ts_constructed_indefinite
};

struct tag_header {
  
  tag_class tclass;
  tag_number number;
  tag_shape shape;
  std::size_t length;
};

constexpr tag_header th_end = { tc_universal, 0, ts_primitive, 0 };

} // end namespace ber

#endif // BERIO_TAG_H
