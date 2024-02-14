#ifndef BERIO_IO_H
#define BERIO_IO_H

#include <cstddef>

namespace ber {

class octet_input {

  public:
  
  virtual ~octet_input();
  
  virtual bool get(unsigned char& o);
  
  virtual std::size_t get(unsigned char* optr, std::size_t len) = 0;
  
  virtual std::size_t skip(std::size_t len) = 0;
};

class octet_output {

  public:
  
  virtual ~octet_output();
  
  virtual bool put(unsigned char o);
  
  virtual std::size_t put(unsigned char const* optr, std::size_t len) = 0;
};

} // end namespace ber

#endif // BERIO_IO_H
