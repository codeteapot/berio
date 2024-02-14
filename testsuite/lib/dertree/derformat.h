#ifndef DERTREE_DERFORMAT_H
#define DERTREE_DERFORMAT_H

#include <istream>
#include <ostream>

#include <berio/tag.h>

class tree_level {

  public:
  
  static tree_level const error;
  
  tree_level(int = 0);
  
  tree_level operator ++ () const;
  
  bool operator == (int) const;
  
  friend std::ostream& operator << (std::ostream&, tree_level const&);
  
  private:
  
  static constexpr int __error_amount = -1;
  static constexpr int __margin_size = 2;
  
  int __amount;
  
  tree_level(tree_level const&);
  
  bool __error() const;
  
  friend std::istream& operator >> (std::istream&, tree_level&);
  friend std::ostream& operator << (std::ostream&, tree_level const&);
};

class tree_label {

  public:
  
  tree_label();
  
  ber::tag_class tclass;
  unsigned long tnumber;
  
  friend std::istream& operator >> (std::istream&, tree_label&);
  friend std::ostream& operator << (std::ostream&, tree_label const&);
};

class tree_primitive {

  public:
  
  friend std::istream& operator >> (std::istream&, tree_primitive&);
  friend std::ostream& operator << (std::ostream&, tree_primitive const&);
};

#endif // DERTREE_DERFORMAT_H
