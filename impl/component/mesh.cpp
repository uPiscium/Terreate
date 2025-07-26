#include <component/mesh.hpp>
#include <component/registry.hpp>

namespace Terreate::Component {
void Mesh::draw() const {
  if (mMesh) {
    mMesh->draw();
  }
}

shared<Mesh> MeshSystem::create() {
  shared<Mesh> component = std::make_shared<Mesh>();
  mRegistry->add(component->getID(), component);
  return component;
}

shared<Mesh> MeshSystem::create(shared<Resource::Mesh> mesh) {
  shared<Mesh> component = this->create();
  component->load(mesh);
  return component;
}

void MeshSystem::destroy(UUID const &id) {
  if (mRegistry->has<Mesh>(id)) {
    mRegistry->remove<Mesh>(id);
  } else {
    throw std::runtime_error("Mesh component with ID " + id.toString() +
                             " does not exist.");
  }
}

void MeshSystem::update(float const &delta) {
  for (auto &[id, component] : mRegistry->getAll<Mesh>()) {
    component->draw();
  }
}
} // namespace Terreate::Component
