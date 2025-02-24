#ifndef BERIO_TAG_H
#define BERIO_TAG_H

#include <berio/io.h>

namespace ber {

/*!
 * \brief Values for the class field of a tag
 *
 * The integer value of each of them corresponds exactly to the encoded value.
 *
 * \ingroup tag-header
 */
enum tag_class {

  /*!
   * \brief Tag class: UNIVERSAL
   */
  tc_universal = 0,
  
  /*!
   * \brief Tag class: APPLICATION
   */
  tc_application = 1,
  
  /*!
   * \brief Tag class: CONTEXT SPECIFIC
   */
  tc_context_specific = 2,
  
  /*!
   * \brief Tag class: PRIVATE
   */
  tc_private = 3
};
/*!
 * \enum ber::tag_class
 * \hideenumvalues
 */

/*!
 * \brief Type used to store a tag number
 *
 * Non-negative fixed-type integer. Despite adapting to the architecture in use, it is expected to
 * be large enough in any case.
 *
 * Note that if the size is not sufficient for an unexpected case, the tag_decode(tag_header&,
 * octet_input&) operation returns ber::tdr_number_overflow.
 *
 * \ingroup tag-header
 */
using tag_number = unsigned int;

/*!
 * \brief Primitive or constructed, with definite or indefinite length
 *
 * A primitive tag always has a definite length.
 *
 * \ingroup tag-header
 */
enum tag_shape {

  /*!
   * \brief Primitive
   *
   * If it has an indefinite length, the operation tag_decode(tag_header&, octet_input&, bool)
   * returns ber::tdr_indefinite_primitive.
   */
  ts_primitive,
  
  /*!
   * \brief Constructed with definite length
   *
   * None of the tags present in the content can satisfy univ::is_eoc(tag_header const&).
   */
  ts_constructed_definite,
  
  /*!
   * \brief Constructed with indefinite length
   *
   * The last tag present in the content must satisfy univ::is_eoc(tag_header const&).
   */
  ts_constructed_indefinite
};

/*!
 * \brief Header of a tag.
 *
 * Determine the class, the number, and whether it is primitive or constructed, with a defined
 * length or not.
 *
 * \ingroup tag-header
 */
struct tag_header {
  
  /*!
   * \brief Class value in the tag identifier
   *
   * The use of a reserved word to name this field is avoided.
   */
  tag_class tclass;
  
  /*!
   * \brief Value of the number in the tag identifier
   *
   * It is expected to have one of the values defined in \ref tag-header-univnums when the class is
   * ber::tc_universal.
   */
  tag_number number;
  
  /*!
   * \brief Indicates whether a tag is primitive or constructed, and has a definite or indefinite
   *        length
   *
   * A tag is constructed when its shape is not ber::ts_primitive,
   *
   * \snippet tag-shape.cxx Is constructed
   *
   * A tag has definite length when its shape is not ber::ts_constructed_indefinite,
   *
   * \snippet tag-shape.cxx Is definite
   */
  tag_shape shape;
  
  /*!
   * \brief Length of tag content when it is definite
   *
   * Its value is irrelevant when the tag shape is ber::ts_constructed_indefinite.
   *
   * The maximum length is determined by the appropriate size type for the architecture. If the tag
   * exceeds the allowed length, the operation tag_decode(tag_header&, octet_input&, bool) returns
   * ber::tdr_length_overflow.
   */
  std::size_t length;
};

/*!
 * \brief Result of decoding a tag
 *
 * Only the values ber::tdr_complete and ber::tdr_empty denote a successful decoding.
 *
 * \ingroup tag-header
 */
enum tag_decode_result {

  /*!
   * \brief Decoding has been successfully completed
   */
  tdr_complete,
  
  /*!
   * \brief There is no octet available when decoding begins
   */
  tdr_empty,
  
  /*!
   * \brief The tag is primitive, but its length is indefinite
   */
  tdr_indefinite_primitive,
  
  /*!
   * \brief There are not enough octets to determine the tag number
   */
  tdr_number_incomplete,
  
  /*!
   * \brief The decoded number is too large to be stored in a destination of type ber::tag_number
   */
  tdr_number_overflow,
  
  /*!
   * \brief The first octet of the number is a zero, but it is not the last one
   *
   * Only if the \p strict parameter of tag_decode(tag_header&, octet_input&, bool) is set to
   * \c true.
   *
   * See section \c 8.1.2.4.2.c of \c X.690-202101.
   */
  tdr_number_zero_leading,
  
  /*!
   * \brief Decoding the number requires additional octets even if it is less than \c 31
   *
   * Only if the \p strict parameter of tag_decode(tag_header&, octet_input&, bool) is set to
   * \c true.
   */
  tdr_number_unneeded_high,
  
  /*!
   * \brief There are no octets available to decode to length.
   */
  tdr_length_undefined,
  
  /*!
   * \brief The length is decoded with \c 127 octets, or perhaps more
   *
   * See section \c 8.1.3.5.c of \c X.690-0207.
   */
  tdr_length_unsupported,
  
  /*!
   * \brief The length requires additional octets not available
   */
  tdr_length_incomplete,
  
  /*!
   * \brief The decoded length is too large to be stored in a destination of type std::size_t
   */
  tdr_length_overflow,
  
  /*!
   * \brief It requires additional octets despite decoding a length less than \c 128
   *
   * Only if the \p strict parameter of tag_decode(tag_header&, octet_input&, bool) is set to
   * \c true.
   */
  tdr_length_unneeded_long
};

/*!
 * \brief Decode a tag header.
 *
 * The result of the operation is known from the return value. The tag has only been correctly
 * decoded if it returns ber::tdr_complete. Otherwise, it returns a different result, or
 * ber::tdr_empty if the input stream has no more octets.
 *
 * Only if the return value is ber::tdr_complete is the result stored in the \p th parameter.
 * Otherwise, the initial value is retained.
 *
 * The input stream is indicated by the parameter \p in. Consumed octets are discarded even when the
 * return value is not ber::tdr_complete.
 *
 * There are three cases where, although it is possible to decode the tag, the encoding rules are
 * not strictly followed. This is denoted by the return value being one of the following values,
 *
 * - ber::tdr_number_zero_leading
 * - ber::tdr_number_unneeded_high
 * - ber::tdr_length_unneeded_long
 *
 * Unless the \p strict parameter is set to \c false, in which case the return value is
 * ber::tdr_complete if the other necessary conditions are met.
 *
 * The following program
 * \include tag-decode.cxx
 *
 * produces the output
 * \include tag-decode.out
 *
 * \param th      Output parameter in which the decoded value is stored.
 * \param in      Stream from which the octets to be decoded are read.
 * \param strict  Determines whether the return value should not be ber::tdr_complete if the
                  encoding rules are not met even if decoding is possible.
 *
 * \return The result of the decoding.
 *
 * \ingroup tag-header
 */
tag_decode_result tag_decode(tag_header& th, octet_input& in, bool strict = true);

static_assert(
  sizeof(std::size_t) >= (4 + sizeof(tag_number) * 8 / 7 + sizeof(std::size_t) + 1) / 8,
  "The maximum size of a tag header exceeds the allowed size");

/*!
 * \brief Length required to encode a tag header
 *
 * \pre
 * The maximum size of the tag header does not exceed the maximum value that a target of type
 * std::size_t can store.
 *
 * The following program
 * \include tag-sizeof.cxx
 *
 * produces the output
 * \include tag-sizeof.out
 *
 * \param th  Tag header on which the length needed for encoding is queried.
 *
 * \return The size needed to encode the tag header.
 *
 * \ingroup tag-header
 */
std::size_t tag_sizeof(tag_header const& th);

static_assert(
  sizeof(std::size_t) < 128,
  "X.609 does not support lengths greater than 2^127");

/*!
 * \brief TODO Documentation
 *
 * \pre The maximum value of std::size_t does not exceed 2<sup>127</sup>.
 *
 * \ingroup tag-header
 */
void tag_encode(tag_header const& th, octet_output& out);

} // end namespace ber

#endif // BERIO_TAG_H
