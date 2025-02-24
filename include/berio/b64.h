#ifndef BERIO_B64_H
#define BERIO_B64_H

/*!
 * \file
 *
 * \brief Input and output streams for Base-64 and their respective canonical traits
 *
 * It contains the ber::octet_input and ber::octet_output adapters for Base-64 character sources and
 * sinks respectively.
 */

#include <cctype>
#include <limits>

#include <berio/io.h>
#include <berio/deliminput.h>

namespace ber {

/*!
 * \brief Reveal and discard the next element from a character source
 *
 * The source is delegated the responsibility of holding the next character even after it has been
 * read. This character may contain bits that will be relevant for the subsequent octet.
 *
 * This is an **unspecialized primary template** that expects the declaration of the following
 * operations,
 *
 * \code
 * static bool peek(SourceT& src, char& c);
 * \endcode
 *
 * Reveals the next character from \c src without discarding it, and assigns its value to \c c.
 * Returns \c true if a character was available; \c false otherwise.
 *
 * \code
 * static void ignore(SourceT& src)
 * \endcode
 *
 * Discard the next character from \c src.
 *
 * \tparam SourceT  The type of character source.
 *
 * \see ber::b64_source_traits<StdInputStreamT>
 *
 * \ingroup in-out-base64
 */
template<typename SourceT>
struct b64_source_traits;

/*!
 * \brief Insert a new element into a character sink
 *
 * This is an **unspecialized primary template** that expects the declaration of the following
 * operation,
 *
 * \code
 * static bool put(SinkT& sink, char c)
 * \endcode
 *
 * Use \c c as the character to insert into \p sink and return \c true if it was possible. Return
 * \c false otherwise.
 *
 * \tparam SinkT  The type of character sink.
 *
 * \see ber::b64_sink_traits<StdOutputStreamT>
 *
 * \ingroup in-out-base64
 */
template<typename SinkT>
struct b64_sink_traits;

/*!
 * \brief Input stream for a Base-64 character source
 *
 * It wraps a character source of type \p SourceT that is treated as defined by \p Traits.
 *
 * \note
 * It also implements delimited_input::skippable, so it can be wrapped in turn by a delimited_input.
 *
 * Characters for which std::isspace(int) evaluates to \c true are automatically discarded.
 *
 * \tparam SourceT  Type of the wrapped character source.
 * \tparam Traits   Operations applied to the character source. By default, it is
 *                  ber::b64_source_traits.
 *
 * \ingroup in-out-base64
 */
template<typename SourceT, typename Traits = b64_source_traits<SourceT>>
class b64_input : public delimited_input::skippable {

  public:
  
  /*!
   * \brief Wraps an existing character source
   *
   * The initial state for the Base-64 to octet conversion is established.
   *
   * \pre
   * The next character from the source is the first of the quartet. Risk of misreading.
   *
   * \param src  The wrapped character source.
   */
  b64_input(SourceT& src) noexcept:
  __src(src),
  __last_s(0),
  __get_fn(&b64_input::__get_first) {}
  
  /*!
   * \brief Copy constructor disabled
   *
   * Preservation of the conversion state is not implemented.
   */
  b64_input(b64_input const&) = delete;
  
  std::size_t getn(unsigned char* optr, std::size_t len) override final {
    std::size_t actual_len = 0;
    while (actual_len < len and (this->*__get_fn)(optr + actual_len))
      ++actual_len;
    return actual_len;
  }
  
  std::size_t skip(std::size_t len) override final {
    unsigned char null;
    std::size_t actual_len = 0;
    while (actual_len < len and (this->*__get_fn)(&null))
      ++actual_len;
    return actual_len;
  }

  private:
  
  struct alphabet_sextet_table {
  
    static constexpr std::size_t array_size = 128;
    static constexpr unsigned char invalid_sextet = 0xc0;
    
    unsigned char array[array_size];
    
    constexpr alphabet_sextet_table():
    array() {
      for (std::size_t i = 0; i < array_size; ++i) {
        char c = static_cast<char>(i);
        if (c >= 'A' and c <= 'Z')
          array[i] = c - 'A';
        else if (c >= 'a' and c <= 'z')
          array[i] = c - 'a' + 26;
        else if (c >= '0' and c <= '9')
          array[i] = c - '0' + 52;
        else if (c == '+')
          array[i] = 62;
        else if (c == '/')
          array[i] = 63;
        else
          array[i] = invalid_sextet;
      }
    }
    
    unsigned char operator [] (char c) const {
      return c > 0 ? array[c] : invalid_sextet;
    }
  };
  
  static constexpr alphabet_sextet_table __as_table {};
  
  SourceT& __src;
  unsigned char __last_s;
  bool (b64_input::*__get_fn)(unsigned char*);
  
  void __to_first() {
    __last_s = 0; // Not strictly needed
    __get_fn = &b64_input::__get_first;
  }
  
  void __to_second(unsigned char last_s) {
    __last_s = last_s;
    __get_fn = &b64_input::__get_second;
  }
  
  void __to_third(unsigned char last_s) {
    __last_s = last_s;
    __get_fn = &b64_input::__get_third;
  }
  
  bool __get_first(unsigned char* optr) {
    char c;
    if (not __source_peek(c))
      return false;
    unsigned char const s1 = __as_table[c];
    if (s1 == alphabet_sextet_table::invalid_sextet) // Including '='
      return false;
    Traits::ignore(__src);
    if (not __source_peek(c))
      return false;
    unsigned char const s2 = __as_table[c];
    if (s2 == alphabet_sextet_table::invalid_sextet) // Including '='
      return false;
    Traits::ignore(__src);
    *optr = s1 << 2 bitor s2 >> 4;
    __to_second(s2);
    return true;
  }
  
  bool __get_second(unsigned char* optr) {
    char c;
    if (not __source_peek(c))
      return false;
    if (c == '=') {
      Traits::ignore(__src);
      Traits::ignore(__src); // Assumes that next character is '='
      return false;
    }
    unsigned char const s = __as_table[c];
    if (s == alphabet_sextet_table::invalid_sextet)
      return false;
    Traits::ignore(__src);
    *optr = __last_s << 4 bitor s >> 2;
    __to_third(s);
    return true;
  }
  
  bool __get_third(unsigned char* optr) {
    char c;
    if (not __source_peek(c))
      return false;
    if (c == '=') {
      Traits::ignore(__src);
      return false;
    }
    unsigned char const s = __as_table[c];
    if (s == alphabet_sextet_table::invalid_sextet) // Including '='
      return false;
    Traits::ignore(__src);
    *optr = __last_s << 6 bitor s;
    __to_first();
    return true;
  }
  
  bool __source_peek(char& c) {
    while (Traits::peek(__src, c)) {
      if (not std::isspace(c))
        return true;
      Traits::ignore(__src);
    }
    return false;
  }
};

/*!
 * \brief Output stream for a Base-64 character sink
 *
 * It wraps a character sink of type \p SinkT that is treated as defined by \p Traits.
 *
 * It is possible to set a line limit using the \c line_len property. There is no limit if its
 * value is zero. This involves a character counter whose initial value is zero, and which returns
 * to zero when the next character exceeds the limit.
 *
 * \note
 * **Rationale**: It makes no sense for a line to have a limit of zero characters; therefore, this
 * value is used to indicate that there is no limit.
 *
 * The \c line_sep property points to the \a C-string—a null-terminated character string—that
 * defines the line separator. It is added when the character that exceeds the line limit is about
 * to be written, never before.
 *
 * \tparam SinkT   Type of the wrapped character sink.
 * \tparam Traits  Operations applied to the character sink. By default, it is ber::b64_sink_traits.
 *
 * \ingroup in-out-base64
 */
template<typename SinkT, typename Traits = b64_sink_traits<SinkT>>
class b64_output : public octet_output {

  public:

  /*!
   * \brief Wraps an existing character sink
   *
   * The initial state for the Base-64 from octet conversion is established.
   *
   * \pre
   * The next character in the character sink will be the first of the quartet. Risk of incorrect
   * writing.
   *
   * \param sink      The wrapped character source.
   * \param line_len  Length of the line, or zero if it has no limit.
   * \param line_sep  Sequence of characters \a C-string that defines the line separator.
   */
  b64_output(SinkT& sink, std::size_t line_len = 0, char const* line_sep = "\r\n") noexcept:
  __sink(sink),
  __line_len(line_len),
  __line_sep(line_sep),
  __index(0),
  // Not actually needed: __last_o(0)
  __put_fn(&b64_output::__put_first),
  __conclude_fn(&b64_output::__conclude_first) {}
  
  /*!
   * \brief Copy constructor disabled
   *
   * Preservation of the conversion state is not implemented.
   */
  b64_output(b64_output const&) = delete;

  std::size_t putn(unsigned char const* optr, std::size_t len) override final {
    std::size_t actual_len = 0;
    while (actual_len < len and (this->*__put_fn)(optr + actual_len))
      ++actual_len;
    return actual_len;
  }
  
  /*!
   * \brief Write the padding characters
   *
   * And returns to the initial state of conversion from Base-64 to octets.
   */
  void conclude() {
    (this->*__conclude_fn)();
  }

  private:
  
  struct sextet_alphabet_table {
  
    static constexpr std::size_t array_size = 64;
    
    unsigned char array[array_size];
    
    constexpr sextet_alphabet_table():
    array() {
      for (std::size_t i = 0; i < array_size; ++i) {
        unsigned char s = static_cast<unsigned char>(i);
        if (s >= 0 and s <= 25)
          array[i] = s + 'A';
        else if (s >= 26 and s <= 51)
          array[i] = s - 26 + 'a';
        else if (s >= 52 and s <= 61)
          array[i] = s - 52 + '0';
        else if (s == 62)
          array[i] = '+';
        else
          array[i] = '/'; // PRE s < 64, so s = 63 here
      }
    }
    
    unsigned char operator [] (unsigned char s) const { // PRE s < 64
      return array[s];
    }
  };
  
  static constexpr sextet_alphabet_table __sa_table {};
  
  SinkT& __sink;
  std::size_t const __line_len;
  char const* const __line_sep;
  std::size_t __index;
  unsigned char __last_o;
  bool (b64_output::*__put_fn)(unsigned char const*);
  void (b64_output::*__conclude_fn)();
  
  void __to_first() {
    // Not actually needed: __last_o = 0
    __put_fn = &b64_output::__put_first;
    __conclude_fn = &b64_output::__conclude_first;
  }
  
  void __to_second(unsigned char last_o) {
    __last_o = last_o;
    __put_fn = &b64_output::__put_second;
    __conclude_fn = &b64_output::__conclude_second;
  }
  
  void __to_third(unsigned char last_o) {
    __last_o = last_o;
    __put_fn = &b64_output::__put_third;
    __conclude_fn = &b64_output::__conclude_third;
  }
  
  bool __put_first(unsigned char const* optr) {
    unsigned char const o = *optr;
    unsigned char const s = __sa_table[o >> 2];
    if (__sink_put(s)) {
      __to_second(o);
      return true;
    }
    return false;
  }
  
  bool __put_second(unsigned char const* optr) {
    unsigned char const o = *optr;
    unsigned char const s = __sa_table[((__last_o bitand 0x03) << 4) bitor (o >> 4)];
    if (__sink_put(s)) {
      __to_third(o);
      return true;
    }
    return false;
  }
  
  bool __put_third(unsigned char const* optr) {
    unsigned char const o = *optr;
    unsigned char const s1 = __sa_table[((__last_o bitand 0x0f) << 2) bitor (o >> 6)];
    unsigned char const s2 = __sa_table[o bitand 0x3f];
    if (__sink_put(s1) and __sink_put(s2)) {
      __to_first();
      return true;
    }
    return false;
  }
  
  void __conclude_first() {
    // Nothing to be do
  }
  
  void __conclude_second() {
    unsigned char const s = __sa_table[(__last_o bitand 0x03) << 4];
    if (__sink_put(s) and __sink_put('=') and __sink_put('='))
      __to_first();
  }
  
  void __conclude_third() {
    unsigned char const s = __sa_table[(__last_o bitand 0x0f) << 2];
    if (__sink_put(s) and __sink_put('='))
      __to_first();
  }
  
  bool __sink_put(char c) {
    if (__line_len > 0 and ++__index > __line_len) {
      __index = 1;
      for (char const* cptr = __line_sep; *cptr not_eq '\0'; ++cptr)
        if (not Traits::put(__sink, *cptr))
          return false;
    }
    return Traits::put(__sink, c);
  }
};

} // end namespace ber

#endif // BERIO_B64_H
