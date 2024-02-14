#ifndef BERIO_DELIMINPUT_H
#define BERIO_DELIMINPUT_H

#include <berio/io.h>
#include <berio/tag.h>

namespace ber {

class delimited_input : public octet_input {

  public:
  
  struct segment {
    std::size_t end;
    bool definite;
  };
  
  class delimit_sentry {

    public:

    delimit_sentry(delimited_input& in, tag_length const& length);
    ~delimit_sentry() noexcept;

    private:

    delimited_input& __in;
    segment const __delim;
  };

  delimited_input(octet_input& in);
  
  segment delimit(tag_length const& length);
  
  void delimit(segment const& delim);
  
  std::size_t get(unsigned char* optr, std::size_t len);
  
  std::size_t skip(std::size_t len);

  private:

  std::size_t __available(std::size_t) const;
  std::size_t __remaining() const;

  octet_input& __in;
  std::size_t __off;
  segment __delim;
};

} // end namespace ber

#endif // BERIO_DELIMINPUT_H
