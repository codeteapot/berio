#ifndef BERTREE_BERPRINT_H
#define BERTREE_BERPRINT_H

#include <istream>

#include <berio/io.h>

#include "berlabel.h"
#include "berlevel.h"

enum ber_print_result {
  bpr_remaining,
  bpr_finished,
  bpr_failed
};

template<typename CharT, typename Traits>
ber_print_result ber_print(
    ber::octet_output&,
    std::basic_istream<CharT, Traits>&,
    ber_level const&);
    
template<typename CharT, typename Traits>
void ber_print(ber::octet_output&& out, std::basic_istream<CharT, Traits>& is) {
  ber_level level;
  if (is >> level and level == 0)
    ber_print(out, is, level);
  out.flush();
  std::cout << std::endl;
}

template<typename CharT, typename Traits>
ber_print_result ber_print(
    ber::octet_output& out,
    std::basic_istream<CharT, Traits>& is,
    ber_level const& level) {
  ber::tag_header th;
  ber_label label;
  if (not (is >> label))
    return bpr_failed;
  th.tclass = label.tclass();
  th.number = label.number();
  if (is >> std::ws >> ber_content::begin) {
    if (not label.definite())
      return bpr_failed;
    th.shape = ber::ts_primitive;
    th.length = 0;
    // TODO Primitive
    is >> ber_content::end;
    tag_encode(th, out);
    return bpr_remaining;
  }
  is.clear();
  if (label.definite()) {
    th.shape = ber::ts_constructed_definite;
  }
  else {
    th.shape = ber::ts_constructed_indefinite;
    th.length = 0;
    tag_encode(th, out);
    for (
        ber_print_result bpr = bpr_remaining;
        bpr not_eq bpr_finished;
        bpr = ber_print(out, is, level + 1)) {
      ber_level next_level;
      if (bpr == bpr_failed or not (is >> next_level))
        return bpr_failed;
      if (next_level == level)
        return bpr_finished;
      if (next_level == level + 1)
        continue;
      return bpr_failed;
    }
  }
  return bpr_remaining;
}

#endif // BERTREE_BERPRINT_H
