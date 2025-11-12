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
 * \brief Primitive or constructed, definite or indefinite
 *
 * A primitive tag is always definite.
 *
 * \ingroup tag-header
 */
enum tag_shape {

  /*!
   * \brief Primitive (definite)
   *
   * Note that if a primitive tag is indefinite, the tag_decode(tag_header&, octet_input& in)
   * operation returns ber::tdr_indefinite_primitive.
   */
  ts_primitive,
  
  /*!
   * \brief Constructed definite
   */
  ts_constructed_definite,
  
  /*!
   * \brief Constructed indefinite
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
  
  tag_class tclass;
  tag_number number;
  
  /*!
   * TODO Documentation: definite := not ts_constructed_indefinite; constructed := not ts_primitive
   */
  tag_shape shape;
  std::size_t length;
};

/*!
 * \ingroup tag-header
 */
bool operator == (tag_header const&, tag_header const&);

/*!
 * \ingroup tag-header
 */
constexpr tag_header th_end = { tc_universal, 0, ts_primitive, 0 };

/*!
 * \ingroup tag-header
 */
enum tag_decode_result {

  tdr_complete,
  tdr_empty,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_indefinite_primitive,
  tdr_number_incomplete,
  
  /*!
   * \brief TODO Documentation
   */
  tdr_number_overflow,
  tdr_length_undefined,
  tdr_length_unsupported,
  tdr_length_incomplete,
  tdr_length_overflow
};

/*!
 * \brief Decode a tag header.
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
