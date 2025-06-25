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
  
  bool end() const;
  
  void print(ber::octet_output&) const;
  
  protected:
  
  ber_printer_node(ber::tag_class, ber::tag_number);
  
  virtual ber::tag_shape shape() const = 0;
  virtual std::size_t length() const = 0;
  virtual void content_print(ber::octet_output&) const = 0;
  
  private:
  
  ber::tag_class __tclass;
  ber::tag_number __number;
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
  ber_printer_node* __node;
  
  void __replace(ber_level const&, ber_printer_node*);
};

template<typename ContentT>
struct ber_printer_content_traits {

  static size_t tag_sizeof(ContentT content) { return 0; }
  
  static void tag_encode(ber::octet_output& out, ContentT content) {}
};

template<>
struct ber_printer_content_traits<ber_bit_string> {

  static size_t tag_sizeof(ber_bit_string const& content) { return 0; }
  
  static void tag_encode(ber::octet_output& out, ber_bit_string const& content) {}
};

template<typename ContentT>
class ber_printer_primitive : public ber_printer_node {

  public:
  
  ber_printer_primitive(ber::tag_class tclass, ber::tag_number number, ContentT& content):
  ber_printer_node(tclass, number),
  __content(content) {}
  
  protected:
  
  ber::tag_shape shape() const {
    return ber::ts_primitive;
  }
  
  std::size_t length() const {
    return 0; // TODO ...
  }
  
  void content_print(ber::octet_output& out) const {
    ber_printer_content_traits<ContentT>::tag_encode(out, __content);
  }
  
  private:
  
  ContentT __content;
};

template<typename CharT, typename Traits>
class ber_printer_indefinite : public ber_printer_node {

  public:
  
  ber_printer_indefinite(
      ber::tag_class tclass,
      ber::tag_number number,
      ber_level const& level,
      std::basic_istream<CharT, Traits>& is):
  ber_printer_node(tclass, number),
  __level(level),
  __is(is) {}
  
  protected:
  
  ber::tag_shape shape() const {
    return ber::ts_constructed_indefinite;
  }
  
  std::size_t length() const {
    return 0;
  }
  
  void content_print(ber::octet_output& out) const {
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
  if (not (is >> std::ws))
    return;
  label.do_extract(is);
  if (not is)
    return;
  if (is >> std::ws >> ber_content::begin) {
    if (not label.definite())
      return; // Invalid
    if (is >> ber_content::end) {
      ber_bit_string bit_str;
      __replace(level, new ber_printer_primitive(label.tclass(), label.number(), bit_str));
    }
    else
      return; // Invalid
  }
  is.clear();
  if (label.definite()) {
    // TODO Definite: Recursive
    return;
  }
  __replace(level, new ber_printer_indefinite(label.tclass(), label.number(), level, is));
}

template<typename CharT, typename Traits>
void ber_print(ber::octet_output&& out, std::basic_istream<CharT, Traits>& is) {
  ber_printer printer;
  if (is >> printer)
    printer.print(out);
  out.flush();
}

#endif // BERTREE_BERPRINT_H
