#ifndef DERTREE_DERTREE_H
#define DERTREE_DERTREE_H

#include <istream>
#include <ostream>

#include <berio/deliminput.h>

void tree_scan(ber::octet_input&, std::ostream&);
void tree_print(ber::octet_output&, std::istream&);

class der_tree {

  private:
  
  class der_tree_level {
  
    public:
    
    template<class charT, class Traits>
    friend std::basic_ostream<charT, Traits>& operator << (
        std::basic_ostream<charT, Traits>& os,
        der_tree_level const& level) {
      // TODO ...
      return os;
    }
  };
  
  class der_tree_label {
  
    public:
    
    template<class charT, class Traits>
    friend std::basic_ostream<charT, Traits>& operator << (
        std::basic_ostream<charT, Traits>& os,
        der_tree_label const& label) {
      // TODO ...
      return os;
    }
  };
  
  class der_tree_node {
  
    public:
    
    class scanner {
    
      public:
      
      scanner(ber::delimited_input&, der_tree_level const&);
    };
    
    der_tree_node() = delete;
    
    private:
    
    der_tree_level __level;
    der_tree_label __label;
    ber::delimited_input& __in;
    
    template<class charT, class Traits>
    friend std::basic_ostream<charT, Traits>& operator << (
        std::basic_ostream<charT, Traits>& os,
        scanner const& scn) {
      // TODO ...
      return os;
    } 
  };

  public:
  
  class scanner {
  
    public:
    
    explicit scanner(ber::octet_input&);
    
    private:
    
    template<class charT, class Traits>
    friend std::basic_ostream<charT, Traits>& operator << (
        std::basic_ostream<charT, Traits>& os,
        scanner const& scn) {
      if (not os.good())
        return os;
      typename std::basic_ostream<charT, Traits>::sentry sty(os);
      if (sty) {
        // TODO ...
      }
      return os;
    }
  };
  
  class printer {
  
    // TODO ...
  };
  
  der_tree() = delete;
};

#endif // DERTREE_DERTREE_H
