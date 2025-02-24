#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>

#include <berio/deliminput.h>
#include <berio/encoding.h>
#include <berio/b64istream.h>
#include <berio/b64ostream.h>
#include <berio/univ.h>

#include "bertree.h"

using namespace std;
using namespace ber;
using namespace ber::univ;

bool arg_exist(int, char*[], char const*);
char const* basename_get(char*);

enum ber_scan_result {
  bsr_remaining,
  bsr_finished,
  bsr_failed
};

template<typename CharT, typename Traits>
ber_scan_result ber_scan(
    delimited_input& in,
    basic_ostream<CharT, Traits>& os,
    ber_level const& level) {
  tag_header th = th_end;
  switch (tag_decode(th, in)) {
    case tdr_empty:
      return bsr_finished;
    case tdr_complete:
      bool const definite = th.shape not_eq ts_constructed_indefinite;
      os << level << ' ' << ber_label(th.tclass, th.number, definite);
      delimited_input::sentry const sty(in, definite, th.length);
      if (th.shape == ts_primitive) {
        bool end = false;
        string sep = "";
        os << ' ' << ber_content::begin;
        if (th.tclass == tc_universal)
          switch (th.number) {
            case tn_eoc:
              end = true;
              break;
            case tn_boolean:
              tag_decode_boolean(in, [&os](bool b) { os << (b ? "true" : "false"); });
              // TODO DER restriction X.690 11.1
              break;
            case tn_integer:
              tag_decode_integer<int>(in, [&os](int i) { os << i; }); // TODO BUG 255 gnu-cert
              break;
            case tn_bit_string:
              tag_decode_bit_string<int>(in, [&os, &sep](int b) {
                os << sep << b;
                sep = " ";
              });
              break;
            case tn_utf8_string:
              tag_decode_utf8_string(in, [&os](char32_t c) { os << c; });
              break;
            case tn_object_ident:
              tag_decode_oid<unsigned>(in, [&os, &sep](unsigned i) {
                os << sep << i;
                sep = ".";
              });
              break;
            case ber::univ::tn_print_string:
            case ber::univ::tn_utc_time:
              tag_decode_print_string<char>(in, [&os](char c) { os << c; });
              break;
            default:
              tag_decode_octet_string<int>(in, [&os, &sep](int i) {
                os << sep << hex << setfill('0') << setw(2) << i << dec;
                sep = " ";
              });
          }
        else {
	  string sep = "";
	  tag_decode_octet_string<int>(in, [&os, &sep](int i) {
	    os << sep << hex << setfill('0') << setw(2) << i << dec;
	    sep = " ";
	  });
	}
	os << ber_content::end << endl;
        return os.good() ? end ? bsr_finished : bsr_remaining : bsr_failed;
      }
      os << endl;
      for (
          ber_scan_result bsr = bsr_remaining;
          bsr not_eq bsr_finished;
          bsr = ber_scan(in, os, level + 1))
        if (bsr == bsr_failed or not os.good())
          return bsr_failed;
  }
  return bsr_remaining;
}

int main(int argc, char* argv[]) {
  if (arg_exist(argc, argv, "-s")) {
    b64_istream_source b64_cin_src(cin);
    b64_input b64_cin(b64_cin_src);
    delimited_input delim_b64_cin(b64_cin);
    ber_scan(delim_b64_cin, cout, 0);
    return EXIT_SUCCESS;
  }
  if (arg_exist(argc, argv, "-p")) {
    b64_ostream_target b64_cout_tgt(cout);
    b64_output b64_cout(b64_cout_tgt);
    // TODO ...
    return EXIT_SUCCESS;
  }
  cout << "Usage: " << basename_get(argv[0]) << " (-s|-p)" << endl;
  return EXIT_FAILURE;
}

bool arg_exist(int argc, char* argv[], char const* argn) {
  return find_if(
    argv + 1,
    argv + argc,
    [argn] (char const* str) { return strcmp(str, argn) == 0; }) not_eq argv + argc;
}

char const* basename_get(char* path) {
  char const* c_name = strrchr(path, '/');
  return c_name == nullptr ? path : c_name + 1; // PRE Not ended with '/'
}
