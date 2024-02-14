#ifndef BERTREE_BERFORMAT_H
#define BERTREE_BERFORMAT_H

#include <istream>
#include <ostream>

#include <berio/tag.h>

class tree_level {

  public:
  
  tree_level(int = 0);
  
  tree_level operator ++ () const;
  
  bool operator == (int) const;
  
  private:
  
  static constexpr int __margin_size = 2;
  
  int __amount;
  
  tree_level(tree_level const&);
  
  friend std::istream& operator >> (std::istream&, tree_level&);
  
  friend std::ostream& operator << (std::ostream&, tree_level const&);
};

class tree_label {

  public:
  
  tree_label();
  
  ber::tag_class tclass;
  unsigned long tnumber;
  
  friend std::ostream& operator << (std::ostream&, tree_label const&);
};

class tree_definite {

  public:
  
  tree_definite(bool = true);
  
  private:
  
  bool __value;
  
  friend std::ostream& operator << (std::ostream&, tree_definite const&);
};

#endif // BERTREE_BERFORMAT_H
