#pragma once

#include "../common/type.hpp"
#include "../resource/mesh.hpp"

#include "interface.hpp"

namespace Terreate::Component {
class Mesh : public IComponent {
private:
  UUID mID;
  shared<Resource::Mesh> mMesh;

public:
  Mesh() = default;
  ~Mesh() override = default;

  UUID const &getID() const override { return mID; }

  void load(shared<Resource::Mesh> const &mesh) { mMesh = mesh; }
  void draw() const;
};

class MeshSystem : public IComponentSystem {
private:
  PROHIBIT_COPY_AND_ASSIGN(MeshSystem);

private:
  UUID mID;
  shared<ComponentRegistry> mRegistry;

public:
  MeshSystem() = default;
  ~MeshSystem() override = default;

  UUID const &getID() const override { return mID; }

  shared<Mesh> create();
  shared<Mesh> create(shared<Resource::Mesh> mesh);
  void destroy(UUID const &id);
  void inject(shared<ComponentRegistry> registry) override {
    mRegistry = registry;
  }
  void update(float const &delta) override;
};

} // namespace Terreate::Component
