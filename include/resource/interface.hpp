#pragma once

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
  virtual void inject(shared<ResourceRegistry> registry) = 0;
};

template <typename T>
concept ResourceManager = extends<T, IResourceManager>;

} // namespace Terreate::Resource
