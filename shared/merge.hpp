#pragma once

#include "./register.hpp"
#include "merge/shared/merge-api.h"

#include <span>
#include <vector>

namespace Merge {
namespace {

Merge::API::MergeFieldDefinition createFieldFromMetadata(FieldTemplate f) {
  Merge::API::MergeFieldDefinition fieldDef;

  fieldDef.name = f.name;
  fieldDef.type = f.ty;
  // TODO: Offset

  return fieldDef;
}



void createTypeFromMetadata(ImageIndex image, TypeTemplate const &t) {
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
    auto fieldDef = createFieldFromMetadata(f);

    fields.emplace_back(fieldDef);
  }

  for (auto const &m : t.methods) {
    Merge::API::MergeMethodDefinition methodDef = createMethodFromMetadata(m);

    methods.emplace_back(methodDef);
  }

  std::array<Merge::API::MergeTypeDefinition, 1> types{poggersDef};

  auto tyIdx = Merge::API::AppendTypes(std::span(types));
  Merge::API::CreateMethods(image, tyIdx, methods);
  Merge::API::CreateFields(image, tyIdx, fields);
  Merge::API::CreateProperties(image, tyIdx, {});
}

void AutoRegister(std::string_view name) {
  AssemblyIndex assembly = Merge::API::CreateAssembly(name);
  ImageIndex image =
      Merge::API::CreateImage(assembly, std::string(name) + ".dll");

  for (auto &[_, t] : ::Merge::Registry::types) {
    createTypeFromMetadata(image, t);
  }
}
} // namespace

} // namespace Merge