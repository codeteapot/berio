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
   *
   * It has the value 0.
   */
  tc_universal = 0,
  
  /*!
   * \brief Tag class: APPLICATION
   *
   * It has the value 1.
   */
  tc_application = 1,
  
  /*!
   * \brief Tag class: CONTEXT SPECIFIC
   *
   * It has the value 2.
   */
  tc_context_specific = 2,
  
  /*!
   * \brief Tag class: PRIVATE
   *
   * It has the value 3.
   */
  tc_private = 3
};

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
typedef unsigned int tag_number;

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
   * Note that if a primitive tag has an indefinite length, the operation tag_decode(tag_header&,
   * octet_input&) returns ber::tdr_indefinite_primitive.
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
 * \brief Characteristics defined in the header of a tag
 *
 * It includes the characteristics corresponding to the identifier of a tag, as well as the length
 * of its content.
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
   * exceeds the allowed length, the operation tag_decode(tag_header&, octet_input&) returns
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
   * \brief TODO Documentation
   */
  tdr_complete,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_empty,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_indefinite_primitive,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_number_incomplete,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_number_overflow,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_length_undefined,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_length_unsupported,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_length_incomplete,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_length_overflow
};

/*!
 * \brief Decode a tag header.
 *
 * TODO Documentation
 *
 * \param th  Output parameter in which the decoded value is stored.
 *
 * \include tag-decode.cxx
 *
 * \ingroup tag-header
 */
tag_decode_result tag_decode(tag_header& th, octet_input& in);

/*!
 * \ingroup tag-header
 */
std::size_t tag_sizeof(tag_header const& th);

/*!
 * \ingroup tag-header
 */
void tag_encode(tag_header const& th, octet_output& out);

} // end namespace ber

#endif // BERIO_TAG_H
