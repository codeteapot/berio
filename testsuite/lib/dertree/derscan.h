#ifndef DERTREE_DERSCAN_H
#define DERTREE_DERSCAN_H

#include <iomanip>

#include <berio/deliminput.h>
#include <berio/univ.h>

#include "dertree.h"

enum der_scan_result {
  dsr_remaining,
  dsr_finished,
  dsr_failed
};

template<typename charT, typename Traits>
der_scan_result der_scan_boolean(ber::octet_input& in, std::basic_ostream<charT, Traits>& os) {
  ber::univ::tag_decode_boolean(in, [&os](bool b) { os << (b ? "true" : "false"); });
  return dsr_finished;
}

template<typename charT, typename Traits>
der_scan_result der_scan_integer(ber::octet_input& in, std::basic_ostream<charT, Traits>& os) {
  ber::univ::tag_decode_integer<int>(in, [&os](int i) { os << i; });
  return dsr_finished;
}

template<typename charT, typename Traits>
der_scan_result der_scan_bit_string(ber::octet_input& in, std::basic_ostream<charT, Traits>& os) {
  std::string sep = "";
  ber::univ::tag_decode_bit_string<int>(in, [&os, &sep](int b) {
    os << sep << b;
    sep = " ";
  });
  return dsr_finished;
}

template<typename charT, typename Traits>
der_scan_result der_scan_utf8_string(ber::octet_input& in, std::basic_ostream<charT, Traits>& os) {
  ber::univ::tag_decode_utf8_string(in, [&os](char32_t c) { os << c; });
  return dsr_finished;
}

template<typename charT, typename Traits>
der_scan_result der_scan_print_string(ber::octet_input& in, std::basic_ostream<charT, Traits>& os) {
  ber::univ::tag_decode_print_string<char>(in, [&os](char c) { os << c; });
  return dsr_finished;
}

template<typename charT, typename Traits>
der_scan_result der_scan_object_ident(ber::octet_input& in, std::basic_ostream<charT, Traits>& os) {
  std::string sep = "";
  ber::univ::tag_decode_oid<unsigned>(in, [&os, &sep](unsigned i) {
    os << sep << i;
    sep = ".";
  });
  return dsr_finished;
}

template<typename charT, typename Traits>
der_scan_result der_scan_default(ber::octet_input& in, std::basic_ostream<charT, Traits>& os) {
  std::ios::fmtflags f = os.flags();
  os.flags(std::ios::hex);
  std::string sep = "";
  ber::univ::tag_decode_octet_string<int>(in, [&os, &sep](int i) {
    os << sep << std::setfill('0') << std::setw(2) << i;
    sep = " ";
  });
  os.flags(f);
  return dsr_finished;
}

template<typename charT, typename Traits>
der_scan_result der_scan(
    ber::delimited_input& in,
    std::basic_ostream<charT, Traits>& os,
    int level) {
  der_tree tree(level);
  tree.decode(in);
  if (tree.empty())
    return dsr_finished;
  os << tree;
  if (tree.error())
    return dsr_failed;
  ber::delimited_input::delimit_sentry const delimit_sentry(in, tree.length());
  ber::tag_ident const& ident = tree.ident();
  if (ident.constructed) {
    os << std::endl;
    der_scan_result dsr = dsr_remaining; 
    while (dsr not_eq dsr_finished) {
      if (dsr == dsr_failed)
        return dsr_failed;
      dsr = der_scan(in, os, level + 1);
    }
  }
  else {
    os << ' ' << '[';
    if ([&in, &os](ber::tag_ident const& ident) {
      if (ident.tclass == ber::tc_universal) {
        switch (ident.tnumber) {
          case ber::univ::tn_boolean:
            return der_scan_boolean(in, os);
          case ber::univ::tn_integer:
            return der_scan_integer(in, os);
          case ber::univ::tn_bit_string:
            return der_scan_bit_string(in, os);
          case ber::univ::tn_utf8_string:
            return der_scan_utf8_string(in, os);
          case ber::univ::tn_print_string:
            return der_scan_print_string(in, os);
          case ber::univ::tn_object_ident:
            return der_scan_object_ident(in, os);
          case ber::univ::tn_utc_time:
            return der_scan_print_string(in, os);
        }
      }
      return der_scan_default(in, os);
    }(ident) == dsr_failed)
      return dsr_failed;
    os << ']' << std::endl;
  }
  return os.good() ? tree.end() ? dsr_finished : dsr_remaining : dsr_failed;
}

#endif // DERTREE_DERSCAN_H
