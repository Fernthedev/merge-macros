#include <cstdint>
#include <string_view>

#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/utils/type-concepts.hpp"

template <typename T> struct CustomTypeInfo;

template <typename T, auto FT> struct CustomTypeFieldInfo;
template <typename T, auto MT> struct CustomTypeMethodInfo;

#define DECLARE_CUSTOM_TYPE(Namespaze, Name, Ty, ParentTy)                     \
  template <> struct BS_HOOKS_HIDDEN CustomTypeInfo<Ty> {                      \
    static constexpr std::string_view namespaze = Namespaze;                   \
    static constexpr std::string_view name = Name;                             \
    using Parent = ParentTy;                                                   \
  }

#define DECLARE_CUSTOM_FIELD(Ty, Name)                                         \
  template <> struct BS_HOOKS_HIDDEN CustomTypeFieldInfo<Ty, &Ty::Name> {      \
    static constexpr std::string_view name = #Name;                            \
    static constexpr std::size_t offset = offsetof(Ty, Name);                  \
    using Owner = Ty;                                                          \
    using Type = decltype(Ty::Name);                                           \
  }
#define DECLARE_CUSTOM_METHOD(Ty, Name)                                        \
  template <> struct BS_HOOKS_HIDDEN CustomTypeMethodInfo<Ty, &Ty::Name> {     \
    static constexpr std::string_view name = #Name;                            \
    static constexpr auto const addr = &Ty::Name;                              \
    using Owner = Ty;                                                          \
    using FnType = decltype(&Ty::Name);                                        \
  }

#define DECLARE_CUSTOM_METHOD_OVERRIDE(Ty, Name, BaseType)                     \
  template <> struct BS_HOOKS_HIDDEN CustomTypeMethodInfo<Ty, &Ty::Name> {     \
    static constexpr std::string_view name = #Name;                            \
    static constexpr auto const addr = &Ty::Name;                              \
    using Owner = Ty;                                                          \
    using FnType = decltype(&Ty::Name);                                        \
    using BaseMethod =                                                         \
        ::il2cpp_utils::il2cpp_type_check::MetadataGetter<&BaseType::Name>;    \
  }

// no custom type requires boxing
template <typename CT>
struct BS_HOOKS_HIDDEN ::il2cpp_utils::il2cpp_type_check::need_box<
    CustomTypeInfo<CT>> {
  constexpr static bool value = false;
};

template <typename CT>
struct BS_HOOKS_HIDDEN ::il2cpp_utils ::il2cpp_type_check ::il2cpp_no_arg_class<
    CT *> {
  using CTInfo = CustomTypeInfo<CT>;

  static inline Il2CppClass *get() {
    static auto klass =
        il2cpp_utils ::GetClassFromName(CTInfo::namespaze, CTInfo::name);
    return klass;
  }
};
