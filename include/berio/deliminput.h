#ifndef BERIO_DELIMINPUT_H
#define BERIO_DELIMINPUT_H

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
 * availability of octets is also subject to the delimiter.
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
 * defined by std::numeric_limits<std::size_t>::max().
 *
 * \ingroup in-out-delimin
 */
class delimited_input : public octet_input {

  public:
  
  /*!
   * \brief Result of narrowing a delimited input stream
   *
   * Indicates whether a narrowing has been applied, and the previous delimiter if so.
   *
   * \see delimited_input::narrow(std::size_t)
   */
  struct narrow_result {
  
    /*!
     * \brief Delimiter before narrowing
     *
     * Relevant only if narrow_result::applied is \c true.
     */
    std::size_t prev_end;
    
    /*!
     * \brief Indicates whether the narrowing has actually been applied
     *
     * The value of the narrow_result::prev_end field is irrelevant if the value of this field is
     * \c false.
     */
    bool applied;
  };
  
  /*!
   * \brief Performs narrow and restore of a delimited input stream within a scope
   *
   * Stores the result of a narrowing when it is created. Restores the previous resulting delimiter
   * if the narrowing has been applied.
   *
   * This sentry can be used in cases where there is no delimiter. In such cases, the narrowing is
   * considered not to have been applied.
   *
   * \note **Rationale**:
   * This allows the sentry to be used homogeneously, without suffering the effect of opening a
   * conditional scope.
   *
   * This sentry evaluates to \c true if, and only if, the narrowing has been applied.
   */
  class sentry {

    public:
    
    /*!
     * \brief Narrows the specified input stream and stores the result
     *
     * It does not attempt to narrow if \c delim is \c false. In this case, narrowing is considered
     * not to have been applied.
     *
     * \param in     Input stream to narrow and restore.
     * \param len    Length to be narrowed.
     * \param delim  Determine if the narrowing will be applied.
     *
     * \see delimited_input::narrow(std::size_t)
     * \see narrow_result::applied
     */
    sentry(delimited_input& in, std::size_t len, bool delim) noexcept;
    
    /*!
     * \brief Restores the associated input stream according to the stored narrowing result
     *
     * Nothing happens if \c delim was \c false when the sentry was created or when the narrowing
     * was not actually applied.
     *
     * \see delimited_input::restore(std::size_t)
     * \see narrow_result::applied
     */
    ~sentry();
    
    /*!
     * \brief Indicates whether the associated input stream has actually been narrowed
     *
     * It can be \c false for two reasons, in the creation of the sentry:
     * * It has been specified that \c delim is \c false
     * * The operation delimited_input::narrow(std::size_t) on \c in returns a result whose
     * narrow_result::applied field is \c false
     *
     * \note
     * If the parameter \c delim was \c true, then the sentry evaluating to \c false means that the
     * narrowing could not be applied.
     *
     * The delimited_input::restore(std::size_t) operation is only called in the destructor if the
     * sentry evaluates to \c true.
     */
    operator bool () const noexcept;
    
    /*!
     * \brief The assignment operator is deleted
     *
     * The previous stream delimiter is inevitably restored when the sentry is destroyed.
     */
    sentry& operator = (sentry const&) = delete;

    private:
    
    delimited_input& __in;
    narrow_result const __nr;
  };
  
  /*!
   * \brief Input stream that allows skipping octets
   *
   * Input streams wrapped by ber::delimited_input must implement this interface. The remaining
   * octets are skipped when delimited_input::restore(std::size_t) is called.
   */
  class skippable : public octet_input {
  
    public:
    
    /*!
     * \brief Delegate destruction to implementation
     *
     * It does nothing.
     */
    virtual ~skippable();
    
    /*!
     * \brief Skips the requested number of octets from the stream
     *
     * It waits until the requested number of octets has been discarded using the \p len parameter.
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
  
  /*!
   * \brief Wrap an input stream to limit its length
   *
   * \param in  The wrapped input stream.
   */
  delimited_input(skippable& in) noexcept;
  
#ifdef LIBBERIO_UNIT_TESTING_ENABLED
  delimited_input(skippable&, std::size_t, std::size_t) noexcept;
#endif
  
  /*!
   * \brief Reduce the input stream delimiter
   *
   * The new delimiter is the current offset plus the value of \p len.
   *
   * If the new delimiter exceeds the current delimiter, the narrowing is considered not to have
   * been applied, indicating this with the value \c false in the returned narrow_result::applied
   * field. In this case, it is not necessary to restore the previous delimiter.
   *
   * \note
   * If it is intended to read *recklessly* only up to what the previous delimiter allowed --the
   * rest available--, there is no need to do anything: the delimiter has not been changed.
   *
   * If, and only if, it has been possible to narrow the input stream, the current delimiter is
   * updated and the narrow_result::applied field in the return value is \c true. The
   * narrow_result::prev_end field corresponds to the previous delimiter, which will be used by
   * delimited_input::restore(std::size_t).
   *
   * \param len  Length to which it is wanted to narrow from the current offset.
   *
   * @see sentry::sentry(delimited_input&, std::size_t, bool)
   */
#ifdef LIBBERIO_UNIT_TESTING_ENABLED
  virtual
#endif
  narrow_result narrow(std::size_t len) noexcept;
  
  /*!
   * \brief It skips the currently delimited octets and restores the previous delimiter
   *
   * Call skippable::skip(std::size_t) on the wrapped input stream to skip the ignored octets.
   *
   * \note
   * The trade-off is that this operation is not idempotent if it is restored to an arbitrary
   * delimiter.
   *
   * The parameter \p prev_end corresponds to the narrow_result::prev_end field obtained in the
   * previous call to delimited_input::narrow(std::size_t).
   *
   * \param prev_end  Previous delimiter.
   *
   * @see sentry::~sentry()
   */
#ifdef LIBBERIO_UNIT_TESTING_ENABLED
  virtual
#endif
  void restore(std::size_t prev_end);
  
  /*!
   * \brief Delegate this operation to the wrapped input stream
   *
   * By calling octet_input::getn(unsigned char*, std::size_t) from the wrapped input stream with
   * the specified parameters \p optr and \p len.
   *
   * Additionally, it determines the availability of octets according to the current offset and
   * delimiter.
   *
   * The following program
   * \include delimin-getn.cxx
   *
   * produces the output
   * \include delimin-getn.out
   */
  std::size_t getn(unsigned char* optr, std::size_t len) override final;
  
#ifdef LIBBERIO_UNIT_TESTING_ENABLED
  std::size_t current_off() const noexcept;
  std::size_t current_end() const noexcept;
#endif
  
  private:
  
  std::size_t __remaining() const;

  skippable& __in;
  std::size_t __off, __end; // INVARIANT __off <= __end
};

} // end namespace ber

#endif // BERIO_DELIMINPUT_H
