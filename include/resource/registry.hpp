#pragma once

#include "interface.hpp"

namespace Terreate::Resource {
class ResourceRegistry {
private:
  PROHIBIT_COPY_AND_ASSIGN(ResourceRegistry);

private:
  umap<typeindex, umap<UUID, shared<IResource>>> mResources;

public:
  ResourceRegistry() = default;
  ~ResourceRegistry() = default;

  template <Resource T> shared<T> get(UUID const &uuid) const;
  template <Resource T> umap<UUID, shared<T>> getAll() const;
  template <Resource T> bool has(UUID const &uuid) const;
  template <Resource T> void load(shared<T> resource);
  template <Resource T> void unload(UUID const &uuid);
};
} // namespace Terreate::Resource

// Implementation of ResourceRegistry methods
namespace Terreate::Resource {
template <Resource T> shared<T> ResourceRegistry::get(UUID const &uuid) const {
  auto typeIndex = typeindex(typeid(T));
  if (this->has<T>(uuid)) {
    auto it = mResources.at(typeIndex).find(uuid);
    if (it != mResources.at(typeIndex).end()) {
      return std::dynamic_pointer_cast<T>(it->second);
    }
  }
  return nullptr;
}
template <Resource T> umap<UUID, shared<T>> ResourceRegistry::getAll() const {
  umap<UUID, shared<T>> resources;
  auto typeIndex = typeindex(typeid(T));
  if (mResources.contains(typeIndex)) {
    for (auto &[uuid, resource] : mResources.at(typeIndex)) {
      resources[uuid] = std::dynamic_pointer_cast<T>(resource);
    }
  }
  return resources;
}
template <Resource T> bool ResourceRegistry::has(UUID const &uuid) const {
  auto typeIndex = typeindex(typeid(T));
  if (mResources.contains(typeIndex)) {
    return mResources.at(typeIndex).contains(uuid);
  }
  return false;
}
template <Resource T> void ResourceRegistry::load(shared<T> resource) {
  auto typeIndex = typeindex(typeid(T));
  mResources[typeIndex][resource->getID()] = resource;
}
template <Resource T> void ResourceRegistry::unload(UUID const &uuid) {
  auto typeIndex = typeindex(typeid(T));
  if (mResources.contains(typeIndex)) {
    mResources[typeIndex].erase(uuid);
  }
}
} // namespace Terreate::Resource
