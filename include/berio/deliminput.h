#ifndef BERIO_DELIMINPUT_H
#define BERIO_DELIMINPUT_H

#include <berio/io.h>

namespace ber {

/*!
 * \ingroup in-out-delimin
 */
class delimited_input : public octet_input {

  public:
  
  struct segment {
    std::size_t end;
    bool definite;
  };
  
  class delimit_sentry {

    public:

    delimit_sentry(delimited_input& in, bool definite, std::size_t length) noexcept;
    ~delimit_sentry() noexcept;

    private:
    
    delimited_input& __in;
    segment const __delimiter;
  };
  
  class skippable : public octet_input {
  
    public:
    
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
  
  bool definite() const noexcept;
  
  segment delimit(std::size_t length) noexcept;
  
  void delimit(segment const& delimiter);
  
  std::size_t getn(unsigned char* optr, std::size_t len) override final;

  private:
  
  std::size_t __available(std::size_t) const;
  std::size_t __remaining() const;

  skippable& __in;
  std::size_t __off;
  segment __delimiter;
};

} // end namespace ber

#endif // BERIO_DELIMINPUT_H
