#ifndef BERIO_IO_H
#define BERIO_IO_H

/*!
 * \file
 *
 * \brief Input and output interfaces
 *
 * Define the minimum operations on a stream of octets for encoding and decoding.
 */

#include <cstddef>

namespace ber {

/*!
 * \brief Interface of an octet input stream
 *
 * It allows the sequential reading of octets from an input stream.
 *
 * \ingroup in-out
 */
class octet_input {

  public:
  
  /*!
   * \brief Release the resources associated with the stream
   *
   * It does nothing. It delegates the destruction to the implementation.
   */
  virtual ~octet_input();
  
  /*!
   * \brief Gets the next octet read from the stream
   *
   * A convenience method that is equivalent to
   *
   * \code{.cpp}
   * return getn(&o, 1) > 0;
   * \endcode
   *
   * \param o  Reference on which the read octet is written.
   *
   * \return \c true if the octet has been successfully read; \c false otherwise.
   *
   * \see getn(unsigned char*, std::size_t)
   */
  virtual bool get(unsigned char& o);
  
  /**
   * \brief Gets the requested number of octets to read from the stream
   *
   * It sequentially stores the octets read from position \p optr. In case an exception is thrown,
   * the resulting content spanning the pointed segment is undefined.
   *
   * It waits until it obtains the number of octets requested by the \p len parameter. It gives up
   * when it determines that it will not be able to read all of them. In that case, it is guaranteed
   * that no more octets can be read from the stream.
   *
   * Implementations are free to define whether exceptions are thrown.
   *
   * \param optr  Position from which the read octets are stored.
   * \param len   Number of octets to be read requested.
   *
   * \return The number of octets it has been able to read. It is expected to be less than or equal
   *         to \p len.
   *
   * \note
   * The number of octets returned by the function is given as written in \p optr.
   */
  virtual std::size_t getn(unsigned char* optr, std::size_t len) = 0;
};

/*!
 * \brief Interface of an octet output stream
 *
 * It allows sequential writing of octets to an output stream.
 *
 * \ingroup in-out
 */
class octet_output {

  public:
  
  /*!
   * \brief Release the resources associated with the stream
   *
   * It does nothing. It delegates the destruction to the implementation.
   */
  virtual ~octet_output();
  
  /*!
   * \brief Put an octet into the stream by writing to it
   *
   * A convenience method that is equivalent to
   *
   * \code{.cpp}
   * return putn(&o, 1) > 0;
   * \endcode
   *
   * \param o  Value of the octet to be written.
   *
   * \return \c true if the octet has been successfully written; \c false otherwise.
   *
   * \see putn(unsigned char const*, std::size_t)
   */
  virtual bool put(unsigned char o);
  
  /*!
   * \brief Puts the requested number of octets to write into the stream
   *
   * It sequentially writes the stored octets starting from position \p optr. If an exception
   * occurs, the content written to the stream is undefined.
   *
   * TODO DOCUMENTATION ...
   */
  virtual std::size_t putn(unsigned char const* optr, std::size_t len) = 0;
};

} // end namespace ber

#endif // BERIO_IO_H
