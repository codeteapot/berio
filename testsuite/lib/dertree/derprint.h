#ifndef DERTREE_DERPRINT_H
#define DERTREE_DERPRINT_H

#include <iostream> // TODO Remove it!

#include "dertree.h"

enum der_print_result {
  dpr_finished,
  dpr_failed
};

template<typename CharT, typename Traits>
der_print_result der_print(
    ber::octet_output& out,
    std::basic_istream<CharT, Traits>& is,
    int level) {
  der_tree tree;
  is >> tree;
  if (not is.good())
    return dpr_failed;
  std::clog << tree << std::endl;
  // TODO ...
  // Primitive -> Definite with buffer
  // Primitive known UNIVERSAL -> Definite without buffer
  // Primitive EOC -> Definite empty and decrement level
  // Constructed with EOF -> Indefinite with buffer
  // Constructed without EOF -> Definite with buffer
  tree.encode(out);
  return dpr_finished;
}

#endif // DERTREE_DERPRINT_H
