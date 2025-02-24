#ifndef BERIO_DELIMINPUT_H
#define BERIO_DELIMINPUT_H

#include <limits>

/*!
 * \file
 *
 * \brief Definition of the delimited input and its internal collaborators
 *
 * Although it has a close conceptual dependency on its elements, it does not depend on tag.h.
 */

#include <berio/io.h>

namespace ber {

/*!
 * \brief Wrapper to delimit input streams
 *
 * A special implementation of an input stream that holds an offset and has a delimiter that defines
 * the available length. Read operations are delegated to the wrapped input stream; however, the
 * availability of octets is subject to the delimiter.
 *
 * \note
 * For consistency, the type used for the offset is std::size_t. Data transmissions of indefinite
 * length, that is, those in which it can be guaranteed in advance that the tag_header::shape is
 * ber::ts_constructed_indefinite and that are not subject to the length of a parent tag, should
 * avoid using this type of stream, relegating its use to the first occurrence of an inner tag of
 * definite length.
 *
 * A delimited input stream can be narrowed, but never widened. Each time a delimiter is restored,
 * the currently delimited octets are discarded. This introduces the need for wrapped input streams
 * to also implement delimited_input::skippable.
 *
 * \note
 * Although it could, this class deliberately does not implement delimited_input::skippable. This
 * prevents it from wrapping another instance of this type and unnecessarily creating a stack of
 * delimited streams. For scope-based behavior, delimited_input::sentry can be used, which maintains
 * the current delimiter without the need to involve references to wrapped input streams.
 *
 * Initially, this input stream is delimited with the maximum allowed length, the value of which is
 * defined by delimited_input::max_length.
 *
 * \ingroup in-out-delimin
 */
class delimited_input : public octet_input {

  public:
  
  /*!
   * TODO DOCUMENTATION ...
   */
  static constexpr std::size_t max_length = std::numeric_limits<std::size_t>::max();
  
  /*!
   * \brief Performs narrow and restore of a delimited input stream within a scope
   *
   * Stores the delimiter resulting from the call to narrow(std::size_t) with the specified length
   * when it is created. Restores the stored delimiter using restore(std::size_t) when it is
   * destroyed.
   */
  class sentry {

    public:

    sentry(delimited_input& in, std::size_t len) noexcept;
    
    ~sentry() noexcept;

    private:
    
    delimited_input& __in;
    std::size_t const __prev_end;
  };
  
  class skippable : public octet_input {
  
    public:
    
    virtual ~skippable();
    
    /*!
     * \brief Skips the requested number of octets from the stream
     *
     * It waits until the requested number of bytes has been discarded using the \p len parameter.
     * It gives up when it determines that it will not be able to discard them all. In this case, it
     * is guaranteed that no more octets can be read from the stream.
     *
     * Implementations are free to define whether exceptions are thrown.
     *
     * \param len  Number of octets to be skipped requested.
     *
     * \return The number of octets that it has been able to discard. It is expected to be less than
     *         or equal to \p len.
     */
    virtual std::size_t skip(std::size_t len) = 0;
  };

  delimited_input(skippable& in) noexcept;
  
  /*!
   * TODO DOCUMENTATION ...
   *
   * @see sentry
   */
  std::size_t narrow(std::size_t len) noexcept;
  
  /*!
   * TODO DOCUMENTATION ...
   *
   * @see sentry
   */
  void restore(std::size_t prev_end);
  
  std::size_t getn(unsigned char* optr, std::size_t len) override final;

  private:
  
  std::size_t __remaining() const;

  skippable& __in;
  std::size_t __off, __end;
};

} // end namespace ber

#endif // BERIO_DELIMINPUT_H
