#pragma once

#include "../common/interface.hpp"

#include "interface.hpp"
#include "registry.hpp"

namespace Terreate::Resource {
class ResourceController : public Interface::IController {
private:
  PROHIBIT_COPY_AND_ASSIGN(ResourceController);

private:
  shared<ResourceRegistry> mRegistry;
  umap<typeindex, shared<IResourceManager>> mManagers;

public:
  ResourceController() : mRegistry(std::make_shared<ResourceRegistry>()) {}
  ~ResourceController() = default;

  shared<ResourceRegistry> getRegistry() const { return mRegistry; }
  template <ResourceManager T> shared<T> getManager(bool create = false) const;
  template <ResourceManager T> bool hasManager() const;
  template <ResourceManager T> shared<T> createManager();
  template <ResourceManager T> void destroyManager();

  void update(float const &delta) override {}
};
} // namespace Terreate::Resource

// Implementation of ResourceController methods
namespace Terreate::Resource {
template <ResourceManager T>
shared<T> ResourceController::getManager(bool create) const {
  auto typeIndex = typeindex(typeid(T));
  if (mManagers.contains(typeIndex)) {
    return std::dynamic_pointer_cast<T>(mManagers.at(typeIndex));
  }

  if (create) {
    return this->createManager<T>();
  }

  return nullptr;
}
template <ResourceManager T> bool ResourceController::hasManager() const {
  auto typeIndex = typeindex(typeid(T));
  return mManagers.contains(typeIndex);
}
template <ResourceManager T> shared<T> ResourceController::createManager() {
  auto typeIndex = typeindex(typeid(T));
  if (mManagers.contains(typeIndex)) {
    return std::dynamic_pointer_cast<T>(mManagers.at(typeIndex));
  }
  shared<T> manager = std::make_shared<T>();
  manager->inject(mRegistry);
  mManagers[typeIndex] = manager;
  return manager;
}
template <ResourceManager T> void ResourceController::destroyManager() {
  auto typeIndex = typeindex(typeid(T));
  if (mManagers.contains(typeIndex)) {
    mManagers.erase(typeIndex);
  }
}
} // namespace Terreate::Resource
