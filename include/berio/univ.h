#ifndef BERIO_UNIV_H
#define BERIO_UNIV_H

#include <berio/tag.h>

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

} // end namespace ber::univ
} // end namespace ber

#endif // BERIO_UNIV_H
