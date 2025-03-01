#ifndef __TERREATE_RESOURCE_HPP__
#define __TERREATE_RESOURCE_HPP__

#include <core/uuid.hpp>
#include <exceptions.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;

template <typename T> class Resource {
private:
  Core::UUID mUUID;
  T *mResource = nullptr;

public:
  Resource() : mResource(nullptr) {}
  Resource(T *&&resource) : mResource(resource) {}
  Resource(Resource const &other) : mResource(other.mResource) {
    mUUID = Core::UUID::Copy(other.mUUID);
  }
  Resource(Resource &&other) : mResource(std::move(other.mResource)) {
    mUUID = std::move(other.mUUID);
  }
  ~Resource() {}

  Core::UUID const &GetUUID() { return mUUID; }
  T const *Get() const { return mResource; }
  T *Get() { return mResource; }

  Bool IsValid() const { return mResource != nullptr; }

  void Assign(T *&&resource) { mResource = resource; }
  template <typename... Args> void Assign(Args &&...args) {
    this->Delete();
    mResource = new T(std::forward<Args>(args)...);
  }
  void Delete() {
    if (mResource) {
      delete mResource;
    }
  }

  T const &operator*() const { return *mResource; }
  T &operator*() { return *mResource; }

  T const *operator->() const { return mResource; }
  T *operator->() { return mResource; }

  Resource &operator=(Resource const &other) {
    mResource = other.mResource;
    mUUID = Core::UUID::Copy(other.mUUID);
    return *this;
  }
  Resource &operator=(Resource &&other) {
    mResource = std::move(other.mResource);
    mUUID = std::move(other.mUUID);
    return *this;
  }

public:
  template <typename... Args> static Resource<T> Create(Args &&...args) {
    Resource<T> resource;
    resource.Assign(std::forward<Args>(args)...);
    return resource;
  }
};

template <typename T> class ResourceManager {
private:
  using RType = Resource<T>;

private:
  Map<Core::UUID, RType> mResourceMap;

public:
  ResourceManager() {}
  ~ResourceManager() {}

  void Register(RType const &target) {
    if (mResourceMap.find(target.GetUUID()) != mResourceMap.end()) {
      throw Exceptions::ResourceException(
          "Resource object is already registered.");
    }
    mResourceMap.insert({target.GetUUID(), target});
  }
  void Unregister(RType const &target) {
    auto iter = mResourceMap.find(target.GetUUID());
    if (iter == mResourceMap.end()) {
      throw Exceptions::ResourceException("Resource object is not registered.");
    }
    mResourceMap.erase(iter);
  }
};

} // namespace Terreate

#endif // __TERREATE_RESOURCE_HPP__
