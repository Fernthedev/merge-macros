#include <cstdint>
#include <il2cpp-api-types.h>
#include <il2cpp-class-internals.h>
#include <string_view>
#include <unordered_set>
#include <variant>

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
template <typename T> struct CustomTypeInfo;
template <typename T> struct CustomTypeInterfaces;

template <typename T, auto FT> struct CustomTypeFieldInfo;
template <typename T, auto MT> struct CustomTypeMethodInfo;

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

template <typename... Args> auto extractTypes() {
  return std::array<Il2CppType const*, sizeof...(Args)>({::il2cpp_utils::ExtractIndependentType<Args>()...});
}
template <typename... Args> auto extractClasses() {
  return std::array<Il2CppClass const*, sizeof...(Args)>({(::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<Args*>::get())...});
}

struct CTField {
  Il2CppType const *ty;
  std::string_view name;
  std::size_t offset;
};

struct CTMethod {
  Il2CppType const *ret_ty;
  std::vector<Il2CppType const *> parameters;
  std::string_view name;
  std::size_t addr;
};

struct CustomTypeMetadata {
  std::string_view namespaze;
  std::string_view name;
  Il2CppClass const *parent;
  std::vector<Il2CppClass const*> interfaces;

  std::vector<CTField> fields;
  std::vector<CTMethod> methods;

  CustomTypeMetadata(CustomTypeMetadata &&) = delete;
  explicit CustomTypeMetadata(CustomTypeMetadata const &) = default;
  CustomTypeMetadata() = default;
};

// A type trait to extract the return type from a function pointer
template <typename T> struct function_traits;

// Specialization for function pointers
template <typename Ret, typename... Args>
struct function_traits<Ret (*)(Args...)> {
  using return_type = Ret;
};

struct Register {
  using NameKey = std::pair<std::string_view, std::string_view>;
  static inline std::unordered_map<NameKey, CustomTypeMetadata> types;

  template <typename Ty> static CustomTypeMetadata &registerType() {
    using CTData = ::Merge::CustomTypeInfo<Ty>;

    auto pair = NameKey(CTData::namespaze, CTData::name);

    // return if it exists
    auto it = types.find(pair);
    if (it != types.end())
      return it->second;

    CustomTypeMetadata data;
    data.namespaze = CTData::namespaze;
    data.name = CTData::name;
    if constexpr (has_interfaces<Ty>) {
      auto const &interfaces = ::Merge::CustomTypeInterfaces<Ty>::interfaces();
      data.interfaces = std::vector<Il2CppClass const *>(interfaces.begin(),
                                                         interfaces.end());
    }

    // register if needed
    if constexpr (is_custom_type<typename CTData::Parent>) {
      registerType<typename CTData::Parent>();
    }
    data.parent = classof(typename CTData::Parent *);

    return types.emplace(pair, data).first->second;
  }

  template <typename Ty, auto Field> static std::monostate registerField() {
    using CTData = ::Merge::CustomTypeInfo<Ty>;
    using CTFieldData = ::Merge::CustomTypeFieldInfo<Ty, Field>;

    auto& data = registerType<Ty>();

    CTField field;
    field.name = CTFieldData::name;
    field.offset = CTFieldData::offset;
    field.ty =
        ::il2cpp_utils::ExtractIndependentType<typename CTFieldData::Type>();

    data.fields.emplace_back(field);

    return {};
  }

  template <typename Ty, auto Method> static std::monostate registerMethod() {
    using CTData = ::Merge::CustomTypeInfo<Ty>;
    using CTMethodData = ::Merge::CustomTypeMethodInfo<Ty, Method>;

    auto& data = registerType<Ty>();

    CTMethod method;
    method.name = CTMethodData::name;
    // TODO: Addr
    // method.addr = reinterpret_cast<std::size_t>(reinterpret_cast<void*>(CTMethodData::addr));
    // TODO: Ret
    // method.ret_ty = ::il2cpp_utils::ExtractIndependentType<
    //     typename function_traits<decltype(Method)>::return_type>();

    // TODO: Params

    data.methods.emplace_back(method);

    return {};
  }
};
} // namespace Merge
