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
 * \ingroup in-out
 */
class octet_input {

  public:
  
  virtual ~octet_input();
  
  virtual bool get(unsigned char& o);
  
  virtual std::size_t getn(unsigned char* optr, std::size_t len) = 0;
  
  virtual std::size_t skip(std::size_t len) = 0;
};

/*!
 * \ingroup in-out
 */
class octet_output {

  public:
  
  virtual ~octet_output();
  
  virtual bool put(unsigned char o);
  
  virtual std::size_t putn(unsigned char const* optr, std::size_t len) = 0;
  
  // TODO DOCUMENTATION Not used in atomic operations
  virtual void flush() = 0;
};

} // end namespace ber

#endif // BERIO_IO_H
