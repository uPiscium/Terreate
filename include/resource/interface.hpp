#pragma once

#include "../common/type.hpp"
#include "../common/uuid.hpp"

#include <typeindex>

namespace Terreate::Resource {
class ResourceRegistry;

class IResource {
public:
  virtual ~IResource() = default;
  virtual UUID const &getID() const = 0;
};

template <typename T>
concept ResourceType = extends<T, IResource>;

class IResourceManager {
public:
  virtual ~IResourceManager() = default;
  virtual UUID const &getID() const = 0;
  virtual void setRegistry(shared<ResourceRegistry> registry) = 0;
};

template <typename T>
concept ResourceManagerType = extends<T, IResourceManager>;

class ResourceRegistry {
private:
  umap<std::type_index, umap<UUID, shared<IResource>>> mResources;

public:
  ResourceRegistry() = default;
  ~ResourceRegistry() = default;

  template <typename T> shared<T> getResource(UUID const &uuid) {
    auto typeIndex = std::type_index(typeid(T));
    if (mResources.find(typeIndex) != mResources.end() &&
        mResources[typeIndex].find(uuid) != mResources[typeIndex].end()) {
      return std::static_pointer_cast<T>(mResources[typeIndex][uuid]);
    }
    return nullptr;
  }

  template <typename T> bool hasResource(UUID const &uuid) {
    auto typeIndex = std::type_index(typeid(T));
    return mResources.find(typeIndex) != mResources.end() &&
           mResources[typeIndex].find(uuid) != mResources[typeIndex].end();
  }

  template <typename T> void loadResource(shared<T> resource) {
    auto typeIndex = std::type_index(typeid(T));
    mResources[typeIndex][resource->getUUID()] = resource;
  }

  template <typename T> void unloadResource(UUID const &uuid) {
    auto typeIndex = std::type_index(typeid(T));
    if (mResources.find(typeIndex) != mResources.end()) {
      mResources[typeIndex].erase(uuid);
    }
  }
};
} // namespace Terreate::Resource
