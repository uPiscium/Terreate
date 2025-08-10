#pragma once

#include "../opengl/shader.hpp"

#include "interface.hpp"
#include "registry.hpp"

namespace Terreate::Resource {
enum class ShaderType : u8 {
  VERTEX,
  FRAGMENT,
  // GEOMETRY,
  // TESSELLATION_CONTROL,
  // TESSELLATION_EVALUATION,
  // COMPUTE
};

class Shader : public IResource {
private:
  UUID mID;
  ShaderType mType;
  str mPath = "";
  str mSource = "";

public:
  Shader(str const &path, ShaderType type);
  ~Shader() override = default;

  UUID const &getID() const override { return mID; }
  ShaderType getType() const { return mType; }
  str const &getPath() const { return mPath; }
  str const &getSource() const { return mSource; }
};

class ShaderManager : public IResourceManager {
private:
  UUID mID;
  umap<str, UUID> mPathIDMap;
  shared<ResourceRegistry> mRegistry;

public:
  ShaderManager() = default;
  ~ShaderManager() override = default;

  UUID const &getID() const override { return mID; }
  UUID const &getIDFor(str const &path) const;
  shared<Shader> get(UUID const &id) const;
  bool has(UUID const &id) const {
    return mRegistry && mRegistry->has<Shader>(id);
  }
  bool has(str const &path) const { return mPathIDMap.contains(path); }

  void inject(shared<ResourceRegistry> registry) override {
    mRegistry = registry;
  }
  shared<Shader> create(str const &path, ShaderType type);
  void destroy(UUID const &id);
  void destroy(str const &path);
};
} // namespace Terreate::Resource
