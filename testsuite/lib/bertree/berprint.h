#ifndef BERTREE_BERPRINT_H
#define BERTREE_BERPRINT_H

#include <istream>

#include <berio/encoding.h>
#include <berio/io.h>
#include <berio/tag.h>

#include "berbitstr.h"
#include "bercontent.h"
#include "berlabel.h"
#include "berlevel.h"

class ber_printer_node {

  public:
  
  virtual ~ber_printer_node();
  
  virtual ber::tag_shape shape() const = 0;
  
  virtual std::size_t length() const = 0;
  
  virtual void print(ber::octet_output&) const = 0;
};

class ber_printer {

  public:
  
  ber_printer();
  ber_printer(ber_printer const&) = delete;
  ~ber_printer();
  
  template<typename CharT, typename Traits>
  void do_extract(std::basic_istream<CharT, Traits>& is);
  
  ber_level const& level() const;
  
  bool end() const;
  
  void print(ber::octet_output&) const;
  
  private:
  
  ber_level __level;
  ber::tag_class __tclass;
  ber::tag_number __number;
  ber_printer_node* __node;
  
  void __replace(ber_level const&, ber::tag_class, ber::tag_number, ber_printer_node*);
};

template<typename ContentT>
class ber_printer_primitive : public ber_printer_node {

  public:
  
  ber_printer_primitive(ContentT& content):
  __content(content) {}
  
  ber::tag_shape shape() const {
    return ber::ts_primitive;
  }
  
  std::size_t length() const {
    return 0; // TODO ...
  }
  
  void print(ber::octet_output&) const {
    // TODO ...
  }
  
  private:
  
  ContentT __content;
};

template<typename CharT, typename Traits>
class ber_printer_indefinite : public ber_printer_node {

  public:
  
  ber_printer_indefinite(ber_level const& level, std::basic_istream<CharT, Traits>& is):
  __level(level),
  __is(is) {}
  
  ber::tag_shape shape() const {
    return ber::ts_constructed_indefinite;
  }
  
  std::size_t length() const {
    return 0;
  }
  
  void print(ber::octet_output& out) const {
    ber_level const child_level = __level + 1;
    ber_printer printer;
    while (__is >> printer) {
      if (printer.level() == child_level) {
        printer.print(out);
        if (printer.end())
          break;
      }
      else
        break; // Invalid
    }
  }
  
  private:
  
  ber_level const __level;
  std::basic_istream<CharT, Traits>& __is;
};

template<typename CharT, typename Traits>
std::basic_istream<CharT, Traits>& operator >> (
    std::basic_istream<CharT, Traits>& is,
    ber_printer& printer) {
  typename std::basic_istream<CharT, Traits>::sentry s(is);
  if (not s)
    return is;
  printer.do_extract(is);
  return is;
}

template<typename CharT, typename Traits>
void ber_printer::do_extract(std::basic_istream<CharT, Traits>& is) {
  ber_level level;
  level.do_extract(is);
  if (not is)
    return;
  ber_label label;
  label.do_extract(is);
  if (not is)
    return;
  if (is >> std::ws >> ber_content::begin) {
    if (not label.definite())
      return; // Invalid
    if (is >> ber_content::end) {
      ber_bit_string bit_str;
      __replace(level, label.tclass(), label.number(), new ber_printer_primitive(bit_str));
    }
    else
      return; // Invalid
  }
  is.clear();
  if (label.definite()) {
    // TODO Definite: Recursive
    return;
  }
  __replace(level, label.tclass(), label.number(), new ber_printer_indefinite(level, is));
}

template<typename CharT, typename Traits>
void ber_print(ber::octet_output&& out, std::basic_istream<CharT, Traits>& is) {
  ber_printer printer;
  if (is >> printer)
    printer.print(out);
  out.flush();
}


enum ber_print_result {
  bpr_remaining,
  bpr_finished,
  bpr_failed
};

template<typename CharT, typename Traits>
ber_print_result ber_print(
    ber::octet_output&,
    std::basic_istream<CharT, Traits>&,
    ber_level const&);
    
/*template<typename CharT, typename Traits>
void ber_print(ber::octet_output&& out, std::basic_istream<CharT, Traits>& is) {
  ber_level level;
  if (is >> level and level == 0)
    ber_print(out, is, level);
  out.flush();
}*/

template<typename CharT, typename Traits>
ber_print_result ber_print(
    ber::octet_output& out,
    std::basic_istream<CharT, Traits>& is,
    ber_level const& level) {
  ber::tag_header th;
  ber_label label;
  if (not (is >> label))
    return bpr_failed;
  th.tclass = label.tclass();
  th.number = label.number();
  if (is >> std::ws >> ber_content::begin) {
    if (not label.definite())
      return bpr_failed;
    th.shape = ber::ts_primitive;
    th.length = 0;
    // TODO Primitive
    is >> ber_content::end;
    ber::tag_encode(th, out);
    return bpr_remaining;
  }
  is.clear();
  if (label.definite()) {
    th.shape = ber::ts_constructed_definite;
  }
  else {
    th.shape = ber::ts_constructed_indefinite;
    th.length = 0;
    ber::tag_encode(th, out);
    for (
        ber_print_result bpr = bpr_remaining;
        bpr not_eq bpr_finished;
        bpr = ber_print(out, is, level + 1)) {
      ber_level next_level;
      if (bpr == bpr_failed or not (is >> next_level))
        return bpr_failed;
      if (next_level == level)
        return bpr_finished;
      if (next_level == level + 1)
        continue;
      return bpr_failed;
    }
  }
  return bpr_remaining;
}

#endif // BERTREE_BERPRINT_H
