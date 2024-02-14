#ifndef DERTREE_DERPRINT_H
#define DERTREE_DERPRINT_H

#include "dertree.h"

enum der_print_result {
  dpr_finished
};

template<typename charT, typename Traits>
der_print_result der_print(
    ber::octet_output& out,
    std::basic_istream<charT, Traits>& is,
    int level) {
  // TODO ...
  return dpr_finished;
}

#endif // DERTREE_DERPRINT_H
