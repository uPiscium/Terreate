#ifndef __TERREATE_RESOURCE_HPP__
#define __TERREATE_RESOURCE_HPP__

#include <core/uuid.hpp>
#include <exceptions.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;

namespace ResourceUtils {
template <typename S> struct ResourceTypeTraits {
  typedef S *Ptr;
  typedef S const *CPtr;
  typedef S &Ref;
  typedef S const &CRef;
};

template <> struct ResourceTypeTraits<void> {
  typedef void *Ptr;
  typedef void const *CPtr;
  typedef void Ref;
  typedef void const CRef;
};

class CounterBase {
private:
  Ulong mCount = 0u;

public:
  CounterBase() : mCount(0u) {}
  CounterBase(Ulong count) : mCount(count) {}
  virtual ~CounterBase() {}
  Ulong const &GetCount() const { return mCount; }
  virtual void Increment() { ++mCount; }
  virtual void Decrement() { --mCount; }
  virtual void Dispose() = 0;
};

template <typename T> class CounterImpl : public CounterBase {
private:
  T *mPtr;

public:
  CounterImpl(T *ptr) : mPtr(ptr) {}
  ~CounterImpl() {}

  void Dispose() override {
    if (mPtr) {
      delete mPtr;
      mPtr = nullptr;
    }
  }
};

template <typename T, typename Deleter>
class CounterImplDeleter : public CounterBase {
private:
  T *mPtr;
  Deleter mDeleter;

public:
  CounterImplDeleter(T *ptr, Deleter deleter) : mPtr(ptr), mDeleter(deleter) {}
  ~CounterImplDeleter() {}

  void Dispose() override {
    if (mPtr) {
      mDeleter(mPtr);
      mPtr = nullptr;
    }
  }
};

class SharedCount {
private:
  CounterBase *mCount;

public:
  SharedCount() : mCount(nullptr) {}
  template <typename T> SharedCount(T *ptr) : mCount(new CounterImpl(ptr)) {}
  template <typename T, typename Deleter>
  SharedCount(T *ptr, Deleter deleter)
      : mCount(new CounterImplDeleter(ptr, deleter)) {}
  SharedCount(SharedCount const &other);
  ~SharedCount();

  Ulong const &GetCount() const { return mCount->GetCount(); }

  void Delete();

  SharedCount &operator=(SharedCount const &other);
};
} // namespace ResourceUtils

template <typename T> class Resource {
private:
  typedef ResourceUtils::SharedCount SharedCount;
  typedef ResourceUtils::ResourceTypeTraits<T> Traits;
  typedef Traits::Ptr Ptr;
  typedef Traits::CPtr CPtr;
  typedef Traits::Ref Ref;
  typedef Traits::CRef CRef;

  template <typename U> friend class Resource;

private:
  Core::UUID mUUID;
  Ptr mResourcePtr = nullptr;
  SharedCount mCounter;

public:
  Resource() : mResourcePtr(nullptr), mCounter() {}
  template <typename U>
  explicit Resource(U *ptr) : mResourcePtr(ptr), mCounter(ptr) {}
  template <typename U, typename Deleter>
  Resource(U *ptr, Deleter deleter)
      : mResourcePtr(ptr), mCounter(ptr, deleter) {}
  template <typename U>
  Resource(Resource<U> const &other)
      : mResourcePtr(other.mResourcePtr), mCounter(other.mCounter) {
    mUUID = Core::UUID::Copy(other.mUUID);
  }
  ~Resource() {}

  Ptr Get() { return mResourcePtr; }
  CPtr Get() const { return mResourcePtr; }
  Core::UUID const &GetUUID() const { return mUUID; }
  Ulong const &GetCount() const { return mCounter.GetCount(); }

  void Delete() { mCounter.Delete(); }

  Ref operator*() {
    if (!mResourcePtr) {
      throw Exceptions::ResourceException("Resource is null");
    }
    return *mResourcePtr;
  }
  CRef operator*() const {
    if (!mResourcePtr) {
      throw Exceptions::ResourceException("Resource is null");
    }
    return *mResourcePtr;
  }
  Ptr operator->() {
    if (!mResourcePtr) {
      throw Exceptions::ResourceException("Resource is null");
    }
    return mResourcePtr;
  }
  CPtr operator->() const {
    if (!mResourcePtr) {
      throw Exceptions::ResourceException("Resource is null");
    }
    return mResourcePtr;
  }

  Resource &operator=(Resource const &other) {
    mResourcePtr = other.mResourcePtr;
    mCounter = other.mCounter;
    mUUID = Core::UUID::Copy(other.mUUID);
    return *this;
  }

public:
  template <typename... Args> static Resource<T> Create(Args &&...args) {
    return Resource<T>(new T(std::forward<Args>(args)...));
  }
};

template <typename T> class ResourceManager {
private:
  typedef Resource<T> Type;

private:
  Map<Str, Core::UUID> mResourceNameMap;
  Map<Core::UUID, Type> mResourceMap;

public:
  ResourceManager() {}
  virtual ~ResourceManager() {}

  Type &Get(Str const &name) {
    auto it = mResourceNameMap.find(name);
    if (it == mResourceNameMap.end()) {
      throw Exceptions::ResourceException("Resource not found");
    }
    return this->Get(it->second);
  }
  Type const &Get(Str const &name) const {
    auto it = mResourceNameMap.find(name);
    if (it == mResourceNameMap.end()) {
      throw Exceptions::ResourceException("Resource not found");
    }
    return this->Get(it->second);
  }
  Type &Get(Core::UUID const &uuid) {
    auto it = mResourceMap.find(uuid);
    if (it == mResourceMap.end()) {
      throw Exceptions::ResourceException("Resource not found");
    }
    return it->second;
  }
  Type const &Get(Core::UUID const &uuid) const {
    auto it = mResourceMap.find(uuid);
    if (it == mResourceMap.end()) {
      throw Exceptions::ResourceException("Resource not found");
    }
    return it->second;
  }

  void Load(Type const &resource, Str const &name = "") {
    Core::UUID uuid = resource.GetUUID();
    if (name.empty()) {
      mResourceNameMap[uuid.ToString()] = uuid;
    } else {
      mResourceNameMap[name] = uuid;
    }
    mResourceMap[uuid] = resource;
  }
  void Unload(Str const &name) {
    auto it = mResourceNameMap.find(name);
    if (it == mResourceNameMap.end()) {
      throw Exceptions::ResourceException("Resource not found");
    }
    this->Unload(it->second);
  }
  void Unload(Core::UUID const &uuid) {
    auto it = mResourceMap.find(uuid);
    if (it == mResourceMap.end()) {
      throw Exceptions::ResourceException("Resource not found");
    }
    mResourceMap.erase(it);
    for (auto it = mResourceNameMap.begin(); it != mResourceNameMap.end();
         ++it) {
      if (it->second == uuid) {
        mResourceNameMap.erase(it);
        break;
      }
    }
  }
};

} // namespace Terreate

#endif // __TERREATE_RESOURCE_HPP__
