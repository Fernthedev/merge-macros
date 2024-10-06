
#include "System/IDisposable.hpp"
#include "System/Object.hpp"
#include "System/String.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "macros.hpp"


struct Test : System::Object {
  int x;
  float Foo();

  //   void Finalize();
};

DECLARE_CUSTOM_TYPE("GlobalNamespace", "Test", Test, System::Object);
DECLARE_CUSTOM_TYPE_INTERFACES(Test, System::IDisposable);
DECLARE_CUSTOM_FIELD(Test, x);
DECLARE_CUSTOM_METHOD(Test, Foo);
DECLARE_CUSTOM_METHOD_OVERRIDE(Test, Finalize, System::Object);

void test() {
  auto klass = classof(Test *);

  auto test =
      il2cpp_utils::RunMethodRethrow<float>(il2cpp_utils::New<Test *>(), "Foo");
}
