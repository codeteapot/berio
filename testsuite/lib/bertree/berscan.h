#ifndef BERTREE_BERSCAN_H
#define BERTREE_BERSCAN_H

#include <istream>
#include <ostream>

#include <berio/deliminput.h>
#include <berio/tag.h>

#include "berbitstr.h"
#include "berboolean.h"
#include "bercontent.h"
#include "berinteger.h"
#include "berlabel.h"
#include "berlevel.h"
#include "beroctetstr.h"
#include "beroid.h"
#include "berprintstr.h"

enum ber_scan_result {
  bsr_remaining,
  bsr_finished,
  bsr_failed
};

template<typename CharT, typename Traits>
ber_scan_result ber_scan(
  ber::delimited_input&,
  std::basic_ostream<CharT, Traits>&,
  ber_level const&);

template<typename CharT, typename Traits>
void ber_scan(ber::octet_input&& in, std::basic_ostream<CharT, Traits>& os) {
  ber::delimited_input delim_in(in);
  ber_scan(delim_in, os, 0);
}

template<typename CharT, typename Traits>
void ber_scan_boolean(ber::delimited_input&, std::basic_ostream<CharT, Traits>&);

template<typename CharT, typename Traits>
void ber_scan_integer(ber::delimited_input&, std::basic_ostream<CharT, Traits>&);

template<typename CharT, typename Traits>
void ber_scan_bit_string(ber::delimited_input&, std::basic_ostream<CharT, Traits>&);

template<typename CharT, typename Traits>
void ber_scan_utf8_string(ber::delimited_input&, std::basic_ostream<CharT, Traits>&);

template<typename CharT, typename Traits>
void ber_scan_oid(ber::delimited_input&, std::basic_ostream<CharT, Traits>&);

template<typename CharT, typename Traits>
void ber_scan_print_string(ber::delimited_input&, std::basic_ostream<CharT, Traits>&);

template<typename CharT, typename Traits>
void ber_scan_octet_string(ber::delimited_input&, std::basic_ostream<CharT, Traits>&);

template<typename CharT, typename Traits>
ber_scan_result ber_scan(
    ber::delimited_input& in,
    std::basic_ostream<CharT, Traits>& os,
    ber_level const& level) {
  ber::tag_header th;
  ber::tag_decode_result tdr = tag_decode(th, in);
  if (tdr == ber::tdr_empty)
    return bsr_finished;
  if (tdr == ber::tdr_complete) {
    bool const definite = th.shape not_eq ber::ts_constructed_indefinite;
    if (in.definite() and (
        // Indefinite within definite. It can be managed, but it is not desired
        not definite or (
        // EOC within definite. It can be managed, but it is not desired
        th.tclass == ber::tc_universal and th.number == ber::univ::tn_eoc and definite)))
      return bsr_failed;
    if (not (os << level << ' ' << ber_label(th.tclass, th.number, definite)))
      return bsr_failed;
    ber::delimited_input::delimit_sentry delimit(in, definite, th.length);
    if (th.shape == ber::ts_primitive) {
      if (not (os << ' ' << ber_content::begin))
        return bsr_failed;
      if (th.tclass == ber::tc_universal)
        switch (th.number) {
          case ber::univ::tn_boolean:
            ber_scan_boolean(in, os);
            break;
          case ber::univ::tn_integer:
            ber_scan_integer(in, os);
            break;
          case ber::univ::tn_bit_string:
            ber_scan_bit_string(in, os);
            break;
          case ber::univ::tn_utf8_string:
            ber_scan_utf8_string(in, os);
            break;
          case ber::univ::tn_object_ident:
            ber_scan_oid(in, os);
            break;
          case ber::univ::tn_print_string:
          case ber::univ::tn_ia5_string:
          case ber::univ::tn_utc_time:
          case ber::univ::tn_gen_time:
            ber_scan_print_string(in, os);
            break;
          default:
            ber_scan_octet_string(in, os);
        }
      else
        ber_scan_octet_string(in, os);
      if (not (os << ber_content::end << std::endl))
        return bsr_failed;
      return th.tclass == ber::tc_universal and th.number == ber::univ::tn_eoc
        ? bsr_finished
        : bsr_remaining;
    }
    os << std::endl;
    for (
        ber_scan_result bsr = bsr_remaining;
        bsr not_eq bsr_finished;
        bsr = ber_scan(in, os, level + 1))
      if (bsr == bsr_failed)
        return bsr_failed;
    return bsr_remaining;
  }
  if (not (os << '!'))
    return bsr_failed;
  if (tdr == ber::tdr_indefinite_primitive)
    os << "indefinite-primitive";
  else if (tdr == ber::tdr_number_incomplete)
    os << "number-incomplete";
  else if (tdr == ber::tdr_number_overflow)
    os << "number-overflow";
  else if (tdr == ber::tdr_length_undefined)
    os << "length-undefined";
  else if (tdr == ber::tdr_length_unsupported)
    os << "length-unsupported";
  else if (tdr == ber::tdr_length_incomplete)
    os << "length-incomplete";
  else if (tdr == ber::tdr_length_overflow)
    os << "length-overflow";
  else
    os << "unknown";
  os << std::endl;
  return bsr_failed;
}

template<typename CharT, typename Traits>
void ber_scan_boolean(ber::delimited_input& in, std::basic_ostream<CharT, Traits>& os) {
  ber_boolean b;
  ber::tag_decode_boolean_result tdbr = ber::tag_decode_boolean(b, in);
  if (tdbr == ber::tdbr_complete)
    os << b;
  else {
    os << '!';
    if (tdbr == ber::tdbr_empty)
      os << "empty";
    else
      os << "unknown";
  }
}

template<typename CharT, typename Traits>
void ber_scan_integer(ber::delimited_input& in, std::basic_ostream<CharT, Traits>& os) {
  ber_integer i;
  ber::tag_decode_integer_result tdir = ber::tag_decode_integer(in, i);
  if (tdir == ber::tdir_complete)
    os << i;
  else {
    os << '!';
    if (tdir == ber::tdir_empty)
      os << "empty";
    else if (tdir == ber::tdir_overflow)
      os << "overflow";
    else
      os << "unknown";
  }
}

template<typename CharT, typename Traits>
void ber_scan_bit_string(ber::delimited_input& in, std::basic_ostream<CharT, Traits>& os) {
  ber_bit_string bstr;
  ber::tag_decode_bit_string(in, bstr);
  os << bstr;
}

template<typename CharT, typename Traits>
void ber_scan_utf8_string(ber::delimited_input& in, std::basic_ostream<CharT, Traits>& os) {
  os << "!not-implemented"; // TODO Implement
}

template<typename CharT, typename Traits>
void ber_scan_oid(ber::delimited_input& in, std::basic_ostream<CharT, Traits>& os) {
  ber_oid oid;
  ber::tag_decode_oid_result tdoidr = ber::tag_decode_oid(in, oid);
  if (tdoidr == ber::tdoidr_complete)
    os << oid;
  else {
    os << '!';
    if (tdoidr == ber::tdoidr_empty)
      os << "empty";
    else if (tdoidr == ber::tdoidr_incomplete)
      os << "incomplete";
    else if (tdoidr == ber::tdoidr_overflow)
      os << "overflow";
    else
      os << "unknown";
  }
}

template<typename CharT, typename Traits>
void ber_scan_print_string(ber::delimited_input& in, std::basic_ostream<CharT, Traits>& os) {
  ber_print_string pstr;
  ber::tag_decode_print_string(in, pstr);
  os << pstr;
}

template<typename CharT, typename Traits>
void ber_scan_octet_string(ber::delimited_input& in, std::basic_ostream<CharT, Traits>& os) {
  ber_octet_string ostr;
  ber::tag_decode_octet_string(in, ostr);
  os << ostr;
}

#endif // BERTREE_BERSCAN_H
