#ifndef BERIO_UNIV_H
#define BERIO_UNIV_H

#include <berio/io.h>

namespace ber {
namespace univ {

constexpr tag_number tn_eoc = 0;

constexpr tag_number tn_boolean = 1;

constexpr tag_number tn_integer = 2;

constexpr tag_number tn_bit_string = 3;

constexpr tag_number tn_octet_string = 4;

constexpr tag_number tn_null = 5;

constexpr tag_number tn_object_ident = 6;

constexpr tag_number tn_object_descr = 7;

constexpr tag_number tn_external = 8;

constexpr tag_number tn_real = 9;

constexpr tag_number tn_enumerated = 10;

constexpr tag_number tn_emb_pdv = 11;

constexpr tag_number tn_utf8_string = 12;

constexpr tag_number tn_reloid = 13;

constexpr tag_number tn_time = 14;

constexpr tag_number tn_sequence = 16;

constexpr tag_number tn_set = 17;

constexpr tag_number tn_num_string = 18;

constexpr tag_number tn_print_string = 19;

constexpr tag_number tn_t61_string = 20;

constexpr tag_number tn_vtex_string = 21;

constexpr tag_number tn_ia5_string = 22;

constexpr tag_number tn_utc_time = 23;

constexpr tag_number tn_gen_time = 24;

constexpr tag_number tn_graph_string = 25;

constexpr tag_number tn_vis_string = 26;

constexpr tag_number tn_gen_string = 27;

constexpr tag_number tn_univ_string = 28;

constexpr tag_number tn_char_string = 29;

constexpr tag_number tn_bmp_string = 30;

constexpr tag_number tn_date = 31;

constexpr tag_number tn_timeofday = 32;

constexpr tag_number tn_datetime = 33;

constexpr tag_number tn_duration = 34;

constexpr tag_number tn_oidiri = 35;

constexpr tag_number tn_reloidiri = 36;

struct iso8601_time_t {

  int fflags;
  int year;
  short month, day, hour, minute, second;
  long nanosecond;
  short offset_hour, offset_minute;
  
  static int const has_date = 0x01;
  static int const has_time = 0x02;
};

template <typename assign_func>
bool tag_decode_boolean(octet_input& in, assign_func assign_fn) {
  unsigned char o;
  if (in.get(o)) {
    assign_fn(o not_eq 0);
    return true;
  }
  return false;
}

template <typename accum_type, typename assign_func>
// C++20: requires std::is_signed<accum_type>
bool tag_decode_integer(octet_input& in, assign_func assign_fn) {
  accum_type accum = 0;
  unsigned char o;
  bool first = true;
  while (in.get(o)) {
    if (not first)
      accum << 8; // TODO Catch overflow
    accum |= o;
    first = false;
  }
  if (not first) {
    assign_fn(accum);
    return true;
  }
  return false;
}

template <typename bit_type, typename push_back_func>
bool tag_decode_bit_string(octet_input& in, push_back_func push_back_fn) {
  unsigned char o;
  if (not in.get(o)) {
    return false;
  }
  // TODO Apply padding
  while (in.get(o)) {
    unsigned char b = o;
    for (int i = 0; i < 8; ++i) {
      push_back_fn(static_cast<bit_type>((b & 0x80) >> 7));
      b = b << 1;
    }
  }
  return true;
}

template <typename octet_type, typename push_back_func>
bool tag_decode_octet_string(octet_input& in, push_back_func push_back_fn) {
  unsigned char o;
  while (in.get(o))
    push_back_fn(static_cast<octet_type>(o));
  return true;
}

// void(char32_t)
template <typename push_back_func>
bool tag_decode_utf8_string(octet_input& in, push_back_func push_back_fn) {
  // TODO Implement
  return true;
}

template <typename char_type>
struct tag_decode_print_string_static_cast_widen_converter {

  constexpr char_type operator()(char c) const { return static_cast<char_type>(c); }
};

template <
    typename char_type,
    typename widen_converter = tag_decode_print_string_static_cast_widen_converter<char_type>,
    typename push_back_func>
bool tag_decode_print_string(octet_input& in, push_back_func push_back_fn) {
  constexpr widen_converter widen_conv;
  unsigned char o;
  while (in.get(o))
    push_back_fn(widen_conv(static_cast<char>(o)));
  return true;
}

template <typename oidc_type, typename push_back_func>
bool tag_decode_oid(octet_input& in, push_back_func push_back_fn) {
  unsigned char o;
  oidc_type si = 0;
  do {
    si = si << 7; // TODO Catch overflow
    if (not in.get(o))
      return false;
    si |= o bitand 0x7f;
  }
  while (o bitand 0x80);
  if (si < 40) {
    push_back_fn(0);
    push_back_fn(si);
  }
  else if (si < 80) {
    push_back_fn(1);
    push_back_fn(si - 40);
  }
  else {
    push_back_fn(2);
    push_back_fn(si - 80);
  }
  while (true) {
    si = 0;
    bool first = true;
    do {
      si = si << 7; // TODO Catch overflow
      if (not in.get(o))
        return first;
      first = false;
      si |= o bitand 0x7f;
    }
    while (o bitand 0x80);
    push_back_fn(si);
  }
}

// void(const iso8601_time_t&)
template <typename assign_func>
bool tag_decode_utc_time(octet_input& in, assign_func assign_fn) {
  // TODO Implement
  return false;
}

} // end namespace ber::univ
} // end namespace ber

#endif // BERIO_UNIV_H
