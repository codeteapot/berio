#ifndef TSTUB_H
#define TSTUB_H

#include <algorithm>
#include <tuple>
#include <vector>

namespace test {

template<typename... Whens>
class stub {

  public:
  
  using return_type = std::common_type_t<typename Whens::return_type...>;
  using args_pack_type = std::common_type_t<typename Whens::args_pack_type...>;
  
  constexpr stub(Whens&&... whens):
  __whens({ std::move(whens)... }) {}
  
  template<typename... Args>
  return_type call(Args... args) {
    args_pack_type const args_pack{ args... };
    __calls.push_back(args_pack);
    return std::apply(
      [args_pack](Whens... whens) {
        return __match_call(args_pack, whens...);
      },
      __whens);
  }
  
  template<typename... Matchers>
  bool verify(Matchers... matchers) const {
    return std::find(
      std::begin(__calls),
      std::end(__calls),
      std::tuple{ matchers... }) not_eq std::end(__calls);
  }
  
  private:
  
  std::tuple<Whens...> const __whens;
  std::vector<args_pack_type> __calls;
  
  template<typename When, typename... RemWhens>
  static return_type __match_call(
      args_pack_type const& args_pack,
      When const& when,
      RemWhens const&... whens) {
    if (when.match(args_pack))
      return when.call();
    if constexpr (sizeof...(whens) > 0)
      return __match_call(args_pack, whens...);
    else
      return return_type{};
  }
};

template<>
class stub<> {

  public:
  
  constexpr stub() {}
};

template<typename CalledWith, typename Answer>
class when {

  public:
  
  using args_pack_type = CalledWith::args_pack_type;
  using return_type = Answer::return_type;
  
  constexpr when(CalledWith&& called_with, Answer&& answer):
  __called_with(std::move(called_with)),
  __answer(std::move(answer)) {}
  
  bool match(args_pack_type const& args_pack) const {
    return __called_with.match(args_pack);
  }
  
  return_type call() const {
    return __answer.call();
  }
  
  private:
  
  CalledWith __called_with;
  Answer __answer;
};

template<typename... Matchers>
class is_called_with {

  private:
  
  template<typename T, typename = void>
  struct matcher_arg {
    using type = T;
  };
  
  template<typename T>
  struct matcher_arg<T, std::void_t<typename T::type>> {
    using type = typename T::type;
  };
  
  public:
  
  using args_pack_type = std::tuple<typename matcher_arg<Matchers>::type...>;
  
  constexpr is_called_with(Matchers... matchers):
  __matchers_pack(std::tuple{ matchers... }) {}
  
  bool match(args_pack_type const& args_pack) const {
    return __matchers_pack == args_pack;
  }
  
  private:
  
  std::tuple<Matchers...> __matchers_pack;
};

template<typename... Args>
class is_called {

  public:
  
  using args_pack_type = std::tuple<Args...>;
  
  constexpr is_called() {}
  
  bool match(args_pack_type const& args_pack) const {
    return true;
  }
};

template<typename F>
class then_answer {

  public:
  
  using return_type = std::invoke_result_t<F>;
  
  constexpr then_answer(F&& fn):
  __fn(std::move(fn)) {}
  
  return_type call() const {
    return __fn();
  }
  
  private:
  
  F __fn;
};

template<typename R>
class then_return {

  public:
  
  using return_type = R;
  
  constexpr then_return(return_type&& ret_val):
  __ret_val(std::move(ret_val)) {}
  
  return_type call() const {
    return __ret_val;
  }
  
  private:
  
  return_type __ret_val;
};

template<>
class then_return<void> {

  public:
  
  using return_type = void;
  
  constexpr then_return() {}
  
  void call() const {}
};

template<typename R, typename X>
class then_throw_impl {

  public:
  
  using return_type = R;
  
  constexpr then_throw_impl(X&& ex):
  __ex(std::move(ex)) {}
  
  return_type call() const {
    throw __ex;
  }
  
  private:
  
  X __ex;
};

template<typename R, typename X>
constexpr auto then_throw(X&& ex) {
  return then_throw_impl<R, std::decay_t<X>>(std::forward<X>(ex));
}

using do_nothing = then_return<void>;

namespace matchers {

template<typename T>
class eq {

  public:
  
  using type = T;
  
  constexpr eq(type&& v): __v(std::move(v)) {}
  
  bool operator == (type const& v) const {
    return __v == v;
  }
  
  private:
  
  T __v;
};

template<typename T>
class any {

  public:
  
  using type = T;
  
  constexpr any() {}
  
  bool operator == (type const&) const {
    return true;
  }
};

}; // namespace matchers

}; // namespace test

#endif // TSTUB_H
