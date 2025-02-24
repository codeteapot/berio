#ifndef BERTREE_BERPRINT_H
#define BERTREE_BERPRINT_H

#include <istream>
#include <stdexcept>
#include <vector>

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
  virtual std::size_t length() const;
  
  protected:
  
  ber_printer_node(ber::tag_header const&);
  virtual void content_print(ber::octet_output&) const = 0;
  
  ber::tag_header __th;
};

class ber_printer {

  public:
  
  ber_printer();
  ber_printer(ber_printer const&) = delete;
  ~ber_printer();
  
  ber_level const& level() const;
  ber_printer_node const* operator -> () const;
  ber_printer_node* detach();
  
  template<typename CharT, typename Traits>
  void do_extract(std::basic_istream<CharT, Traits>& is);
  
  private:
  
  ber_level __level;
  ber_printer_node* __node;
  
  void __replace(ber_level const&, ber_printer_node*);
};

template<typename ContentT>
struct ber_printer_content_traits {

  static size_t tag_sizeof(ContentT content) { return 0; }
  
  static void tag_encode(ContentT content, ber::octet_output& out) {}
};

template<typename ContentT>
class ber_printer_primitive : public ber_printer_node {

  public:
  
  ber_printer_primitive(ber::tag_class tclass, ber::tag_number number, ContentT& content):
  ber_printer_node({
    tclass,
    number,
    ber::ts_primitive,
    ber_printer_content_traits<ContentT>::tag_sizeof(__content)
  }),
  __content(content) {}
  
  protected:

  void content_print(ber::octet_output& out) const {
    ber_printer_content_traits<ContentT>::tag_encode(__content, out);
  }
  
  private:
  
  ContentT __content;
};

class ber_printer_definite : public ber_printer_node {

  public:
  
  ber_printer_definite(ber::tag_class, ber::tag_number);
  virtual ~ber_printer_definite();
  
  void push_back(ber_printer_node*);
  
  protected:
  
  void content_print(ber::octet_output&) const;
  
  private:
  
  std::vector<ber_printer_node*> __children;
};

template<typename CharT, typename Traits>
class ber_printer_indefinite : public ber_printer_node {

  public:
  
  ber_printer_indefinite(
      ber::tag_class tclass,
      ber::tag_number number,
      ber_level const& level,
      std::basic_istream<CharT, Traits>& is):
  ber_printer_node({ tclass, number, ber::ts_constructed_indefinite, 0 }),
  __level(level),
  __is(is) {}

  std::size_t length() const {
    throw std::logic_error("Indefinite has no length");
  }
  
  protected:
  
  void content_print(ber::octet_output& out) const {
    ber_level const child_level = __level + 1;
    ber_printer printer;
    while (__is >> printer) {
      if (printer.level() == child_level) {
        printer->print(out);
        if (printer->end())
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
ber_printer_node* ber_print_octet_string(ber_label const&, std::basic_istream<CharT, Traits>&);

template<typename CharT, typename Traits>
void ber_printer::do_extract(std::basic_istream<CharT, Traits>& is) {
  ber_level level;
  level.do_extract(is);
  if (not is)
    return;
  if (not (is >> std::ws))
    return;
  ber_label label;
  label.do_extract(is);
  if (not is)
    return;
  if (is >> std::ws >> ber_content::begin) {
    if (not label.definite()) {
      is.setstate(std::ios::badbit);
      return; // Invalid
    }
    if (label.tclass() == ber::tc_universal) {
      switch (label.number()) {
        default:
          if (is >> ber_content::end) {
            __replace(level, ber_print_octet_string(label, is));
            return;
          }
      }
    }
    if (is >> ber_content::end) {
      __replace(level, ber_print_octet_string(label, is));
      return;
    }
    is.setstate(std::ios::badbit);
    return; // Invalid
  }
  is.clear();
  if (label.definite()) {
    ber_printer_definite* node = new ber_printer_definite(label.tclass(), label.number());
    __replace(level, node);
    ber_level const child_level = level + 1;
    ber_printer printer;
    while (is >> printer) {
      if (printer.level() == child_level) {
        if (printer->end()) {
          is.setstate(std::ios::badbit);
          return; // Invalid
        }
        node->push_back(printer.detach());
      }
      else if (printer.level() == level)
        return;
      else {
        is.setstate(std::ios::badbit);
        return; // Invalid
      }
    }
    is.clear();
    return;
  }
  __replace(level, new ber_printer_indefinite(label.tclass(), label.number(), level, is));
}

template<typename CharT, typename Traits>
void ber_print(ber::octet_output&& out, std::basic_istream<CharT, Traits>& is) {
  ber_printer printer;
  if (is >> printer)
    printer->print(out);
  out.flush();
}

template<typename CharT, typename Traits>
ber_printer_node* ber_print_octet_string(
    ber_label const& label,
    std::basic_istream<CharT, Traits>& is) {
  ber_bit_string bit_str;
  is >> bit_str;
  return new ber_printer_primitive(label.tclass(), label.number(), bit_str);
}

#endif // BERTREE_BERPRINT_H
