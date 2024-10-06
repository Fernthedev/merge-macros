
#include <string_view>

#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-classes.hpp"
#include "beatsaber-hook/shared/utils/type-concepts.hpp"

namespace std {
template <> struct hash<std::pair<std::string_view, std::string_view>> {
  size_t operator()(
      const std::pair<std::string_view, std::string_view> &key) const noexcept {
    // Combine the hashes of the two string_views
    size_t hash1 = std::hash<std::string_view>{}(key.first);
    size_t hash2 = std::hash<std::string_view>{}(key.second);
    return hash1 ^ (hash2 << 1); // XOR and shift to combine the hashes
  }
};
} // namespace std

namespace Merge {

// A type trait to extract the return type from a function pointer
template <typename T> struct function_traits;

// Specialization for function pointers
template <typename Ret, typename... Args>
struct function_traits<Ret (*)(Args...)> {
  using return_type = Ret;
  using args = std::tuple<Args...>;
};
template <typename Ret, typename T, typename... Args>
struct function_traits<Ret (T::*)(Args...)> {
  using return_type = Ret;
  using args = std::tuple<Args...>;
};

template <typename... Args> auto extractTypes() {
  return std::array<Il2CppType const *, sizeof...(Args)>(
      {::il2cpp_utils::ExtractIndependentType<Args>()...});
}
template <typename... Args> auto extractClasses() {
  return std::array<Il2CppClass const *, sizeof...(Args)>(
      {(::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<
          Args *>::get())...});
}

// Variadic helper function to extract types from a function pointer
template <typename Func, std::size_t... I>
auto extractTypesImplFn(std::index_sequence<I...>) {
  using traits = function_traits<Func>;
  return std::array<Il2CppType const *, sizeof...(I)>{
      il2cpp_utils::ExtractIndependentType<
          std::tuple_element_t<I, typename traits::args>>()...};
}

// Main function to extract types from a function pointer
template <typename Func> auto extractTypesFn() {
  using traits = function_traits<Func>;
  return extractTypesImplFn<Func>(
      std::make_index_sequence<
          std::tuple_size<typename traits::args>::value>{});
}
} // namespace Merge