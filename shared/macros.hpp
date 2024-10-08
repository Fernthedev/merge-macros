#include <cstdint>
#include <il2cpp-api-types.h>
#include <il2cpp-class-internals.h>
#include <string_view>
#include <unordered_set>

#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/utils/type-concepts.hpp"

#include "./register.hpp"

#define DECLARE_CUSTOM_TYPE(Namespaze, Name, Ty, ParentTy)                     \
  template <> struct BS_HOOKS_HIDDEN ::Merge::CustomTypeInfo<Ty> {             \
    static constexpr std::string_view namespaze = Namespaze;                   \
    static constexpr std::string_view name = Name;                             \
    using Parent = ParentTy;                                                   \
                                                                               \
  private:                                                                     \
    static inline ::Merge::CustomTypeMetadata const &data =                    \
        ::Merge::Register::registerType<Ty>();                                 \
  }
#define DECLARE_CUSTOM_TYPE_INTERFACES(Ty, ...)                                \
  template <> struct BS_HOOKS_HIDDEN ::Merge::CustomTypeInterfaces<Ty> {       \
    static auto interfaces() { return ::Merge::ExtractTypes<__VA_ARGS__>(); }  \
  }

#define DECLARE_CUSTOM_FIELD(Ty, Name)                                         \
  template <>                                                                  \
  struct BS_HOOKS_HIDDEN ::Merge::CustomTypeFieldInfo<Ty, &Ty::Name> {         \
    static constexpr std::string_view name = #Name;                            \
    static constexpr std::size_t offset = offsetof(Ty, Name);                  \
    using Owner = Ty;                                                          \
    using Type = decltype(Ty::Name);                                           \
                                                                               \
  private:                                                                     \
    static inline std::monostate fieldData =                                   \
        ::Merge::Register::registerField<Ty, &Ty::Name>();                     \
  }
#define DECLARE_CUSTOM_METHOD(Ty, Name)                                        \
  template <>                                                                  \
  struct BS_HOOKS_HIDDEN ::Merge::CustomTypeMethodInfo<Ty, &Ty::Name> {        \
    static constexpr std::string_view name = #Name;                            \
    static constexpr auto const addr = &Ty::Name;                              \
    using Owner = Ty;                                                          \
    using FnType = decltype(&Ty::Name);                                        \
                                                                               \
  private:                                                                     \
    static inline std::monostate methodData =                                  \
        ::Merge::Register::registerMethod<Ty, &Ty::Name>();                    \
  }

#define DECLARE_CUSTOM_METHOD_OVERRIDE(Ty, Name, BaseType)                     \
  template <>                                                                  \
  struct BS_HOOKS_HIDDEN ::Merge::CustomTypeMethodOverrideInfo<Ty,             \
                                                               &Ty::Name> {    \
                                                                               \
    using BaseMethod =                                                         \
        ::il2cpp_utils::il2cpp_type_check::MetadataGetter<&BaseType::Name>;    \
  }

// no custom type requires boxing
template <typename CT>
struct BS_HOOKS_HIDDEN ::il2cpp_utils::il2cpp_type_check::need_box<
    ::Merge::CustomTypeInfo<CT>> {
  constexpr static bool value = false;
};

template <typename CT>
struct BS_HOOKS_HIDDEN ::il2cpp_utils ::il2cpp_type_check ::il2cpp_no_arg_class<
    CT *> {
  using CTInfo = ::Merge::CustomTypeInfo<CT>;

  static inline Il2CppClass *get() {
    static auto klass =
        il2cpp_utils ::GetClassFromName(CTInfo::namespaze, CTInfo::name);
    return klass;
  }
};
