#ifndef BERIO_B64_H
#define BERIO_B64_H

#include <berio/io.h>

namespace ber {

class b64_input : public octet_input {

  public:
  
  class source { // sink

    public:

    virtual bool peek(char& c) = 0;

    virtual void ignore() = 0;
  };

  b64_input(source& src) noexcept;
  
  std::size_t get(unsigned char* optr, std::size_t len);
  
  std::size_t skip(std::size_t len);

  private:
  
  void __to_first();
  void __to_second(unsigned char last_s);
  void __to_third(unsigned char last_s);
  
  bool __get_first(unsigned char*);
  bool __get_second(unsigned char*);
  bool __get_third(unsigned char*);
  
  source& __src;
  unsigned char __last_s;
  bool (b64_input::*__get_fn)(unsigned char*);
};

class b64_output : public octet_output {

  public:

  class target {
  
    public:
    
    virtual bool put(char c) = 0;
  };

  b64_output(target& tgt) noexcept;

  std::size_t put(unsigned char const* optr, std::size_t len);
  
  void conclude();

  private:
  
  void __to_first();
  void __to_second(unsigned char last_o);
  void __to_third(unsigned char last_o);
  
  bool __put_first(unsigned char const*);
  bool __put_second(unsigned char const*);
  bool __put_third(unsigned char const*);
  
  void __conclude_first();
  void __conclude_second();
  void __conclude_third();

  target& __tgt;
  unsigned char __last_o;
  bool (b64_output::*__put_fn)(unsigned char const*);
  void (b64_output::*__conclude_fn)();
};

} // end namespace ber

#endif // BERIO_B64_H
