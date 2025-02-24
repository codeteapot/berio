#ifndef BERTREE_BERPRINT_H
#define BERTREE_BERPRINT_H

#include <istream>
#include <stdexcept>
#include <vector>

#include <berio/tag.h>

#include "berboolean.h"
#include "bercontent.h"
#include "berlabel.h"
#include "berlevel.h"
#include "beroctetstr.h"

class ber_printer_node {

  public:
  
  virtual ~ber_printer_node();
  
  bool end() const;
  void print(ber::octet_output&) const;
  virtual std::size_t length() const;
  
  protected:
  
  ber_printer_node(ber::tag_header const&);
  void length_add(std::size_t length);
  
  virtual void content_print(ber::octet_output&) const = 0;
  
  private:
  
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

template<typename ContentPolicy>
class ber_printer_primitive : public ber_printer_node {

  public:
  
  ber_printer_primitive(ber::tag_class tclass, ber::tag_number number):
  ber_printer_node({ tclass, number, ber::ts_primitive, 0 }) {}
  
  template<typename CharT, typename Traits>
  void do_extract(std::basic_istream<CharT, Traits>& is) {
    __content.do_extract(is);
    length_add(ContentPolicy::content_sizeof(__content));
  }
  
  protected:
  
  void content_print(ber::octet_output& out) const {
    ContentPolicy::content_encode(out, __content);
  }
  
  private:
  
  typename ContentPolicy::content_type __content;
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

template<typename ContentPolicy, typename CharT, typename Traits>
ber_printer_node* ber_print_primitive(
    ber_label const& label,
    std::basic_istream<CharT, Traits>& is) {
  ber_printer_primitive<ContentPolicy>* node = new ber_printer_primitive<ContentPolicy>(
    label.tclass(),
    label.number());
  node->do_extract(is);
  if (is.eof())
    is.clear();
  if (is >> ber_content::end)
    return node;
  delete node;
  return nullptr;
}

struct ber_printer_boolean_policy {

  using content_type = ber_boolean;
  
  static std::size_t content_sizeof(ber_boolean const& b) {
    return ber::tag_sizeof_boolean(b);
  }
  
  static void content_encode(ber::octet_output& out, ber_boolean const& b) {
    ber::tag_encode_boolean(out, b);
  }
};

struct ber_printer_octet_string_policy {

  using content_type = ber_octet_string;
  
  static std::size_t content_sizeof(ber_octet_string const& ostr) {
    return ber::tag_sizeof_octet_string(ostr);
  }
  
  static void content_encode(ber::octet_output& out, ber_octet_string const& ostr) {
    ber::tag_encode_octet_string(out, ostr);
  }
};

template<typename CharT, typename Traits>
ber_printer_node* ber_print_boolean(
    ber_label const& label,
    std::basic_istream<CharT, Traits>& is) {
  return ber_print_primitive<ber_printer_boolean_policy>(label, is);
}

template<typename CharT, typename Traits>
ber_printer_node* ber_print_octet_string(
    ber_label const& label,
    std::basic_istream<CharT, Traits>& is) {
  return ber_print_primitive<ber_printer_octet_string_policy>(label, is);
}

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
        case ber::univ::tn_boolean:
          __replace(level, ber_print_boolean(label, is));
          return;
        default:
          __replace(level, ber_print_octet_string(label, is));
          return;
      }
    }
    __replace(level, ber_print_octet_string(label, is));
    return;
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

#endif // BERTREE_BERPRINT_H
