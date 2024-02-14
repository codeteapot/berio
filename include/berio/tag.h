#ifndef BERIO_TAG_H
#define BERIO_TAG_H

#include <berio/io.h>

namespace ber {

enum tag_class {

  tc_universal = 0,
  tc_application = 1,
  tc_context_specific = 2,
  tc_private = 3
};

struct tag_ident {

  tag_class tclass;
  bool constructed;
  unsigned long tnumber;
  
  operator bool () const { return tclass not_eq tc_universal or constructed or tnumber not_eq 0; }
};

struct tag_length {

  std::size_t value;
  bool definite;
};

struct tag_header {

  tag_ident ident;
  tag_length length;
};

enum tag_decode_result {

  tdr_complete,
  tdr_empty,
  tdr_incomplete,
  tdr_overflow,
  tdr_invalid
};

std::size_t tag_sizeof(tag_ident const& ident);

std::size_t tag_sizeof(tag_length const& length);

tag_decode_result tag_decode(tag_header& header, octet_input& in);

void tag_encode(tag_header const& header, octet_output& out);

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

#endif // BERIO_TAG_H
