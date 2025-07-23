#pragma once

#include "../common/interface.hpp"
#include "../common/type.hpp"
#include "../common/uuid.hpp"

namespace Terreate::Resource {
class ResourceRegistry;

class IResource {
public:
  virtual ~IResource() = default;
  virtual UUID const &getID() const = 0;
};

template <typename T>
concept Resource = extends<T, IResource>;

class IResourceManager {
public:
  virtual ~IResourceManager() = default;
  virtual UUID const &getID() const = 0;
  virtual void setRegistry(shared<ResourceRegistry> registry) = 0;
};

template <typename T>
concept ResourceManager = extends<T, IResourceManager>;

class ResourceRegistry : public IRegistry {
private:
  umap<typeindex, umap<UUID, shared<IResource>>> mResources;

public:
  ResourceRegistry() = default;
  ~ResourceRegistry() = default;

  template <typename T> shared<T> getResource(UUID const &uuid) {
    auto typeIndex = typeindex(typeid(T));
    if (this->hasResource<T>(uuid)) {
      return std::static_pointer_cast<T>(mResources[typeIndex][uuid]);
    }
    return nullptr;
  }

  template <Resource T> bool hasResource(UUID const &uuid) {
    auto typeIndex = typeindex(typeid(T));
    return mResources.contains(typeIndex) &&
           mResources[typeIndex].contains(uuid);
  }

  template <Resource T> void loadResource(shared<T> resource) {
    auto typeIndex = typeindex(typeid(T));
    mResources[typeIndex][resource->getID()] = resource;
  }

  template <Resource T> void unloadResource(UUID const &uuid) {
    auto typeIndex = typeindex(typeid(T));
    if (mResources.contains(typeIndex)) {
      mResources[typeIndex].erase(uuid);
    }
  }
};
} // namespace Terreate::Resource
