#pragma once

#include <cstdint>
#include <il2cpp-api-types.h>
#include <il2cpp-class-internals.h>
#include <il2cpp-metadata.h>
#include <string_view>
#include <unordered_set>
#include <variant>

#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-classes.hpp"
#include "beatsaber-hook/shared/utils/type-concepts.hpp"

#include "./utils.hpp"

namespace Merge {
template <typename T> struct CustomTypeInfo;
template <typename T> struct CustomTypeInterfaces;

template <typename T, auto FT> struct CustomTypeFieldInfo;
template <typename T, auto MT> struct CustomTypeMethodInfo;
template <typename T, auto MT> struct CustomTypeMethodOverrideInfo;

template <typename T>
concept is_custom_type = requires(T t) {
  typename CustomTypeInfo<T>;                // Check if we can use the type
  typename Merge::CustomTypeInfo<T>::Parent; // Check if Parent type exists
  // Check if the fields exist and are of the expected type
  {
    Merge::CustomTypeInfo<T>::namespaze
  } -> std::convertible_to<std::string_view>;
  { Merge::CustomTypeInfo<T>::name } -> std::convertible_to<std::string_view>;
};

template <typename T>
concept has_interfaces = requires(T t) {
  typename CustomTypeInterfaces<T>; // Check if we can use the type
  { CustomTypeInterfaces<T>::interfaces };
};
template <typename T, auto MT>
concept has_override = requires() {
  typename CustomTypeMethodOverrideInfo<T, MT>; // Check if we can use the type
  { CustomTypeMethodOverrideInfo<T, MT>::BaseMethod };
};

struct FieldTemplate {
  TypeIndex ty;
  std::string_view name;
  std::size_t offset;
};

struct MethodTemplate {
  TypeIndex ret_ty;
  std::vector<TypeIndex> parameters;
  std::string_view name;
  std::size_t addr;
  std::optional<uint16_t> vtableSlot;
};

struct TypeTemplate {
  std::string_view namespaze;
  std::string_view name;
  TypeIndex parent;
  std::vector<TypeIndex> interfaces;

  std::vector<FieldTemplate> fields;
  std::vector<MethodTemplate> methods;

  TypeTemplate(TypeTemplate &&) = delete;
  explicit TypeTemplate(TypeTemplate const &) = default;
  TypeTemplate() = default;
};

struct Registry {
  using NameKey = std::pair<std::string_view, std::string_view>;
  static inline std::unordered_map<NameKey, TypeTemplate> types;

  // register types
  template <is_custom_type Ty>
  static BS_HOOKS_HIDDEN TypeTemplate &registerType() {
    using CTData = ::Merge::CustomTypeInfo<Ty>;

    auto pair = NameKey(CTData::namespaze, CTData::name);

    // return if it exists
    auto it = types.find(pair);
    if (it != types.end())
      return it->second;

    TypeTemplate data;
    data.namespaze = CTData::namespaze;
    data.name = CTData::name;
    if constexpr (has_interfaces<Ty>) {
      auto interfaces = ::Merge::CustomTypeInterfaces<Ty>::interfaces();
      data.interfaces =
          std::vector<TypeIndex>(interfaces.begin(), interfaces.end());
    }

    // register if needed
    if constexpr (is_custom_type<typename CTData::Parent>) {
      registerType<typename CTData::Parent>();
    }
    data.parent = ::Merge::ExtractIndependentType<typename CTData::Parent>();

    return types.emplace(pair, data).first->second;
  }

  template <typename Ty, auto Field>
  static BS_HOOKS_HIDDEN std::monostate registerField() {
    using CTData = ::Merge::CustomTypeInfo<Ty>;
    using CTFieldData = ::Merge::CustomTypeFieldInfo<Ty, Field>;

    auto &data = registerType<Ty>();

    FieldTemplate field;
    field.name = CTFieldData::name;
    field.offset = CTFieldData::offset;
    field.ty = ::Merge::ExtractIndependentType<typename CTFieldData::Type>();

    data.fields.emplace_back(field);

    return {};
  }

  template <typename Ty, auto Method>
  static BS_HOOKS_HIDDEN std::monostate registerMethod() {
    using CTData = ::Merge::CustomTypeInfo<Ty>;
    using CTMethodData = ::Merge::CustomTypeMethodInfo<Ty, Method>;
    using CTOverrideMethodData =
        ::Merge::CustomTypeMethodOverrideInfo<Ty, Method>;

    auto &data = registerType<Ty>();

    MethodTemplate method;
    method.name = CTMethodData::name;

    if constexpr (has_override<Ty, Method>) {
      MethodInfo const *methodInfo =
          CTOverrideMethodData::BaseMethod::methodInfo();
      method.vtableSlot = methodInfo->slot;
    }

    // TODO: Addr
    // method.addr =
    // reinterpret_cast<std::size_t>(reinterpret_cast<void*>(CTMethodData::addr));

    method.ret_ty = ::Merge::ExtractIndependentType<
        typename function_traits<decltype(Method)>::return_type>();

    auto paramArray = extractTypesFnParams<typename CTMethodData::FnType>();
    method.parameters = std::vector(paramArray.begin(), paramArray.end());

    data.methods.emplace_back(method);

    return {};
  }
};
} // namespace Merge
