#include "./register.hpp"
#include "merge/shared/merge-api.h"

#include <span>
#include <vector>

namespace Merge {

static void AutoRegister(std::string_view name) {
  AssemblyIndex assembly = Merge::API::CreateAssembly(name);
  ImageIndex image =
      Merge::API::CreateImage(assembly, std::string(name) + ".dll");

  for (auto &[_, t] : ::Merge::Register::types) {
    Merge::API::MergeTypeDefinition poggersDef;
    poggersDef.name = t.name;
    poggersDef.namespaze = t.namespaze;
    if (t.parent) {
      poggersDef.parent = t.parent;
    }
    poggersDef.interfaces = std::span(t.interfaces);
    poggersDef.attrs = 0;
    poggersDef.typeEnum = IL2CPP_TYPE_CLASS;
    poggersDef.valueType = false;

    std::vector<Merge::API::MergeFieldDefinition> fields;
    fields.reserve(t.fields.size());
    std::vector<Merge::API::MergeMethodDefinition> methods;
    methods.reserve(t.methods.size());

    for (auto const &f : t.fields) {
      Merge::API::MergeFieldDefinition fieldDef;

      fieldDef.name = f.name;
      fieldDef.type = f.ty;
      // TODO: Offset

      fields.emplace_back(fieldDef);
    }

    for (auto const &m : t.methods) {
      Merge::API::MergeMethodDefinition methodDef;
      std::vector<Merge::API::MergeParameterDefinition> params;
      params.reserve(m.parameters.size());

      for (auto const &p : m.parameters) {
        Merge::API::MergeParameterDefinition paramDef;
        paramDef.type = p;
        paramDef.name = "PARAM";
        paramDef.attrs = 0;
      }

      methodDef.name = m.name;
      methodDef.parameters = params;
      methodDef.returnType = m.ret_ty;
      methodDef.flags = 0;
      methodDef.methodPointer = nullptr;

      methods.emplace_back(methodDef);
    }

    std::array<Merge::API::MergeTypeDefinition, 1> types{poggersDef};

    auto tyIdx = Merge::API::AppendTypes(std::span(types));
    Merge::API::CreateMethods(image, tyIdx, methods);
    Merge::API::CreateFields(image, tyIdx, fields);
    Merge::API::CreateProperties(image, tyIdx, {});
  }
}

} // namespace Merge