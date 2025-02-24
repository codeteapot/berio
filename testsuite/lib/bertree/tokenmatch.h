#ifndef BERTREE_TOKENMATCH_H
#define BERTREE_TOKENMATCH_H

#include <algorithm>
#include <istream>

template<typename Tokens, typename CharT, typename Traits>
decltype(std::begin(Tokens::tuples)) token_match(std::basic_istream<CharT, Traits>& is) {
  using char_t = typename Traits::char_type;
  
  std::locale const loc = is.getloc();
  
  std::size_t cand_array_size = std::size(Tokens::tuples);
  std::size_t cand_array[cand_array_size];
  for (std::size_t i = 0; i < cand_array_size; ++i)
    cand_array[i] = i;
    
  constexpr std::size_t buf_size = std::max_element(
    std::begin(Tokens::tuples),
    std::end(Tokens::tuples),
    [](auto& tt1, auto& tt2) { return tt1.token.size() < tt2.token.size(); })->token.size();
  char_t buf[buf_size];
  std::size_t pos = 0;
  for (; pos < buf_size and cand_array_size > 0 and is.get(buf[pos]); ++pos) {
    for (std::size_t cand_index = 0; cand_index < cand_array_size;) {
      auto& token = Tokens::tuples[cand_array[cand_index]].token;
      if (pos < token.size() and token[pos] == is.narrow(buf[pos], '\0'))
        ++cand_index;
      else
        cand_array[cand_index] = cand_array[--cand_array_size];
    }
    if (cand_array_size == 1 and Tokens::tuples[cand_array[0]].token.size() == pos + 1) {
      char_t last_c = Traits::to_char_type(is.peek());
      if (is and std::isspace(last_c, loc))
        return std::begin(Tokens::tuples) + cand_array[0];
      cand_array_size = 0;
    }
  }
  
  for (std::size_t i = pos; i > 0; --i)
    if (not is.putback(buf[i - 1])) {
      is.setstate(std::ios::badbit);
      return std::end(Tokens::tuples);
    }
  is.setstate(std::ios::failbit);
  return std::end(Tokens::tuples);
}

#endif // BERTREE_TOKENMATCH_H
