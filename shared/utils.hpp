#pragma once

#include <cstdint>
#include <il2cpp-metadata.h>
#include <string_view>

#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-classes.hpp"
#include "beatsaber-hook/shared/utils/type-concepts.hpp"
#include "beatsaber-hook/shared/utils/typedefs-array.hpp"
#include "beatsaber-hook/shared/utils/typedefs-string.hpp"

#include "merge/shared/merge-api.h"

#define MERGE_PRIMITIVE_TYPE(ty, namespaze, name)                              \
  template <> struct BS_HOOKS_HIDDEN Merge::il2cpp_metadata_type_index<ty> {   \
    static TypeIndex get() {                                                   \
      static auto typeIdx =                                                    \
          Merge::API::FindTypeDefinitionIndex(namespaze, name);                \
      static Il2CppTypeDefinition const *typeDef =                             \
          Merge::API::GetTypeDefinition(typeIdx);                              \
                                                                               \
      return typeDef->byvalTypeIndex;                                          \
    }                                                                          \
  }

namespace std {
template <>
struct BS_HOOKS_HIDDEN hash<std::pair<std::string_view, std::string_view>> {
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
struct BS_HOOKS_HIDDEN function_traits<Ret (*)(Args...)> {
  using return_type = Ret;
  using args = std::tuple<Args...>;
};
template <typename Ret, typename T, typename... Args>
struct BS_HOOKS_HIDDEN function_traits<Ret (T::*)(Args...)> {
  using return_type = Ret;
  using args = std::tuple<Args...>;
};

// anything else
template <typename T> struct il2cpp_metadata_type_index;

template <typename T>
concept merge_il2cpp_merge_specialized = requires(T t) {
  { il2cpp_metadata_type_index<T>::get() } -> std::convertible_to<TypeIndex>;
};
template <typename T> static TypeIndex ExtractIndependentType();

// TODO: Get type of a ByRef
// Like ExtractType, but only returns an TypeIndex if it can be extracted
// without an instance of T.
template <merge_il2cpp_merge_specialized T>
BS_HOOKS_HIDDEN static TypeIndex ExtractIndependentType() {
  return il2cpp_metadata_type_index<T>::get();
}

BS_HOOKS_HIDDEN static auto ExtractTypes() {
  return ::std::array<TypeIndex, 0>();
}

template <typename... TArgs> BS_HOOKS_HIDDEN static auto ExtractTypes() {
  constexpr std::size_t array_count = sizeof...(TArgs);

  return std::array<TypeIndex, array_count>(
      {ExtractIndependentType<TArgs>()...});
}

// Variadic helper function to extract types from a function pointer
template <typename Func, std::size_t... I>
BS_HOOKS_HIDDEN static auto
extractTypesImplFnParams(std::index_sequence<I...>) {
  using traits = function_traits<Func>;
  return std::array<TypeIndex, sizeof...(I)>{
      ExtractType<std::tuple_element_t<I, typename traits::args>>()...};
}

// Main function to extract types from a function pointer
template <typename Func> BS_HOOKS_HIDDEN static auto extractTypesFnParams() {
  using traits = function_traits<Func>;
  return extractTypesImplFnParams<Func>(
      std::make_index_sequence<
          std::tuple_size<typename traits::args>::value>{});
}
} // namespace Merge

MERGE_PRIMITIVE_TYPE(float, "System", "Single");
MERGE_PRIMITIVE_TYPE(double, "System", "Double");

MERGE_PRIMITIVE_TYPE(int8_t, "System", "Int8");
MERGE_PRIMITIVE_TYPE(int16_t, "System", "Int16");
MERGE_PRIMITIVE_TYPE(int32_t, "System", "Int32");
MERGE_PRIMITIVE_TYPE(int64_t, "System", "Int64");

MERGE_PRIMITIVE_TYPE(uint8_t, "System", "Byte");
MERGE_PRIMITIVE_TYPE(uint16_t, "System", "UInt16");
MERGE_PRIMITIVE_TYPE(uint32_t, "System", "UInt32");
MERGE_PRIMITIVE_TYPE(uint64_t, "System", "UInt64");

MERGE_PRIMITIVE_TYPE(Il2CppObject *, "System", "Object");

MERGE_PRIMITIVE_TYPE(Il2CppString *, "System", "String");
MERGE_PRIMITIVE_TYPE(StringW, "System", "String");
MERGE_PRIMITIVE_TYPE(void *, "System", "IntPtr");

// cordl support
namespace Merge {
template <typename T>
concept il2cpp_merge_type = requires(T t) {
  { T::__IL2CPP_TYPE_DEFINITION_INDEX } -> std::convertible_to<uint32_t>;
};
}

template <Merge::il2cpp_merge_type T>
struct BS_HOOKS_HIDDEN Merge ::il2cpp_metadata_type_index<T> {
  static TypeIndex get() {
    static Il2CppTypeDefinition const *typeDef =
        Merge::API::GetTypeDefinition(T::__IL2CPP_TYPE_DEFINITION_INDEX);

    return typeDef->byvalTypeIndex;
  }
};
