#include <bits/stdc++.h>
#include <utility>

template <typename T, typename... Ts>
struct type_index;

template <typename T, typename First, typename... Rest>
struct type_index<T, First, Rest...> {
  static constexpr std::size_t value = 
    std::is_same<T, First>::value ? 0 : 1 + type_index<T, Rest...>::value;
};

template <std::size_t I, typename... Ts>
struct type_at;

template <typename First, typename...Rest>
struct type_at<0, First, Rest...> {
  using type = First;
};

template <std::size_t I, typename First, typename... Rest>
struct type_at<I, First, Rest...> {
  using type = typename type_at<I - 1, Rest...>::type;
};

template <typename... Ts>
class Variant {
private:
  static constexpr std::size_t max_size = std::max({sizeof(Ts)...});
  static constexpr std::size_t max_align = std::max({alignof(Ts)...});
  using storage_t = typename std::aligned_storage<max_size, max_align>::type;
  
  std::size_t index;
  storage_t buffer;

  template<typename Visitor, std::size_t... Is>
  decltype(auto) visit_impl(Visitor&& vis, std::index_sequence<Is...>) {
    using Fn = decltype(vis);
    using Ret = std::common_type_t<
      decltype(vis(std::declval<typename type_at<Is, Ts...>::type&>()))...
    >;
    Ret result{};
    ((index == Is ? (result = vis(*reinterpret_cast<typename type_at<Is, Ts...>::type*>(&buffer)), 0) : 0), ...);
    return result;
  }

public:
  Variant() : index(0) {
    using T0 = typename type_at<0, Ts...>::type;
    new (&buffer) T0();
  }

  template <typename T>
  Variant(const T& value) {
    index = type_index<T, Ts...>::value;
    new (&buffer) T(value);
  }

  ~Variant() {
    destroy();
  }

  void destroy() {
    visit([](auto& x) {
      using T = std::decay_t<decltype(x)>;
      x.~T();
    });
  }

  std::size_t which() const { return index; }

  template <typename T>
  T& get() {
    if (index != type_index<T, Ts...>::value) {
      throw std::bad_cast();
    }
    return *reinterpret_cast<T*>(&buffer);
  }

  template <typename Visitor>
  decltype(auto) visit(Visitor&& vis) {
    return visit_impl(std::forward<Visitor>(vis), std::index_sequence_for<Ts...>{});
  }
};

int main() {
}
