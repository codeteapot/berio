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

typedef unsigned long tag_number;

struct tag_ident {

  tag_class tclass;
  bool constructed;
  tag_number tnumber;
  
  operator bool () const { return tclass not_eq tc_universal or constructed or tnumber not_eq 0; }
};

struct tag_length {

  std::size_t value;
  bool definite;
};

struct tag_header {

  tag_ident ident;
  tag_length length;
};

} // end namespace ber

#endif // BERIO_TAG_H
