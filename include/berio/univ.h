#ifndef BERIO_UNIV_H
#define BERIO_UNIV_H

#include <berio/tag.h>

namespace ber {
namespace univ {

/*!
 * \brief End-of-contents
 *
 * See section \c 8.1.5 of \c X.690-202102.
 *
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_eoc = 0;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_boolean = 1;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_integer = 2;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_bit_string = 3;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_octet_string = 4;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_null = 5;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_object_ident = 6;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_object_descr = 7;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_external = 8;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_real = 9;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_enumerated = 10;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_emb_pdv = 11;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_utf8_string = 12;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_reloid = 13;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_time = 14;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_sequence = 16;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_set = 17;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_num_string = 18;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_print_string = 19;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_t61_string = 20;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_vtex_string = 21;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_ia5_string = 22;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_utc_time = 23;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_gen_time = 24;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_graph_string = 25;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_vis_string = 26;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_gen_string = 27;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_univ_string = 28;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_char_string = 29;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_bmp_string = 30;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_date = 31;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_timeofday = 32;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_datetime = 33;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_duration = 34;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_oidiri = 35;

/*!
 * \ingroup tag-header-univnums
 */
constexpr tag_number tn_reloidiri = 36;

/*!
 * \brief Determine if the tag header is a universal End-of-contents
 *
* That is, if it is of class ber::tc_universal, if its shape is ber::ts_primitive, and if its number
* is ber::univ::tn_eoc.
*
* The length value is not taken into account.
 *
 * \ingroup tag-header
 */
inline bool is_eoc(tag_header const& th) {
  return th.tclass == tc_universal and th.number == tn_eoc;
}

} // end namespace ber::univ
} // end namespace ber

#endif // BERIO_UNIV_H
