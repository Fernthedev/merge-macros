
#include <cstdint>
#include <il2cpp-metadata.h>
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

template <typename T>
concept il2cpp_merge_type = requires(T t) {
  { T::__IL2CPP_TYPE_INDEX } -> std::convertible_to<uint32_t>;
};

// TODO: Get type of a ByRef
// Like ExtractType, but only returns an Il2CppType* if it can be extracted
// without an instance of T.
template <il2cpp_merge_type T>
const TypeIndex ExtractIndependentType() {
  auto const &logger = il2cpp_utils::Logger;

  return T::__IL2CPP_TYPE_INDEX;
}



inline auto ExtractTypes() { return ::std::array<const Il2CppType *, 0>(); }

template <typename... TArgs> auto ExtractTypes() {
  constexpr std::size_t array_count = sizeof...(TArgs);

  return std::array<TypeIndex, array_count>({ExtractIndependentType<TArgs>()...});
}

// Variadic helper function to extract types from a function pointer
template <typename Func, std::size_t... I>
auto extractTypesImplFn(std::index_sequence<I...>) {
  using traits = function_traits<Func>;
  return std::array<Il2CppType const *, sizeof...(I)>{
      ExtractType<std::tuple_element_t<I, typename traits::args>>()...};
}

// Main function to extract types from a function pointer
template <typename Func> auto extractTypesFn() {
  using traits = function_traits<Func>;
  return extractTypesImplFn<Func>(
      std::make_index_sequence<
          std::tuple_size<typename traits::args>::value>{});
}
} // namespace Merge