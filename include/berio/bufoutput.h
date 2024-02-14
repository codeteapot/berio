#ifndef BERIO_BUFOUTPUT_H
#define BERIO_BUFOUTPUT_H

#include <berio/io.h>

namespace ber {

class buffered_output : public octet_output {

  public:
  
  buffered_output(octet_output& out);
  
  virtual ~buffered_output();
  
  std::size_t length() const;
  
  std::size_t put(unsigned char const* optr, std::size_t len);
  
  void flush();
  
  private:
  
  class buffer_node {
  
    public:
    
    buffer_node();
    
    virtual void dump(octet_output&, std::size_t) const = 0;
    virtual void dump(octet_output&) const = 0;
    virtual std::size_t copy(unsigned char const*, std::size_t, std::size_t) = 0;
    
    buffer_node* next() const;
    buffer_node* next(buffer_node*);
    
    private:
    
    buffer_node* __next;
  };
  
  template <std::size_t array_size>
  class buffer_array : public buffer_node {
  
    public:
    
    void dump(octet_output&, std::size_t) const;
    void dump(octet_output&) const;
    std::size_t copy(unsigned char const*, std::size_t, std::size_t len); // PRE len <= array_size
    
    private:
    
    unsigned char __array[array_size];
  };
  
  template <std::size_t>
  void __allocate(void (buffered_output::*)());
  
  void __allocate();
  void __allocate_256b();
  void __allocate_1kb();
  void __allocate_4kb();
  
  octet_output& __out;
  buffer_node* __first;
  buffer_node* __last;
  std::size_t __last_off, __len;
  void (buffered_output::*__allocate_fn)();
};

} // end namespace ber

#endif // BERIO_BUFOUTPUT_H
