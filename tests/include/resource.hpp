#pragma once
#include <exception.hpp>
#include <type.hpp>

namespace Terreate::Core::ResourceUtils {
using namespace Terreate::Types;
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
  u64 mCount = 0u;

public:
  CounterBase();
  CounterBase(u64 const &count);
  virtual ~CounterBase();
  u64 const &GetCount() const;
  virtual void Increment();
  virtual void Decrement();
  virtual void Dispose() = 0;
  virtual operator bool() const = 0;
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

  operator bool() const override { return mPtr != nullptr; }
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

  operator bool() const override { return mPtr != nullptr; }
};

class SharedCount {
private:
  CounterBase *mCount;

public:
  SharedCount();
  template <typename T> SharedCount(T *ptr) : mCount(new CounterImpl(ptr)) {}
  template <typename T, typename Deleter>
  SharedCount(T *ptr, Deleter deleter)
      : mCount(new CounterImplDeleter(ptr, deleter)) {}
  SharedCount(SharedCount const &other);
  ~SharedCount();

  u64 const &GetCount() const;

  void Delete();

  SharedCount &operator=(SharedCount const &other);
  operator bool() const;
};
} // namespace Terreate::Core::ResourceUtils

namespace Terreate::Core {
using namespace Terreate::Types;

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
  Ptr mResourcePtr = nullptr;
  SharedCount mCounter;

public:
  Resource() : mResourcePtr(nullptr), mCounter() {}
  Resource(std::nullptr_t) : mResourcePtr(nullptr), mCounter() {}
  Resource(Resource const &other)
      : mResourcePtr(other.mResourcePtr), mCounter(other.mCounter) {}
  Resource(Resource &&other)
      : mResourcePtr(other.mResourcePtr), mCounter(other.mCounter) {
    other.mResourcePtr = nullptr;
  }
  template <typename U>
  explicit Resource(U *ptr) : mResourcePtr(ptr), mCounter(ptr) {}
  template <typename U, typename Deleter>
  Resource(U *ptr, Deleter deleter)
      : mResourcePtr(ptr), mCounter(ptr, deleter) {}
  template <typename U>
  Resource(Resource<U> const &other)
      : mResourcePtr(other.mResourcePtr), mCounter(other.mCounter) {}
  ~Resource() {}

  Ptr Get() { return mResourcePtr; }
  CPtr Get() const { return mResourcePtr; }
  u64 const &GetCount() const { return mCounter.GetCount(); }

  void Delete() {
    if (mResourcePtr != nullptr) {
      mCounter.Delete();
    }
  }

  Ref operator*() {
    if (!mResourcePtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mResourcePtr;
  }
  CRef operator*() const {
    if (!mResourcePtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mResourcePtr;
  }
  Ptr operator->() {
    if (!mResourcePtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mResourcePtr;
  }
  CPtr operator->() const {
    if (!mResourcePtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mResourcePtr;
  }

  Resource &operator=(Resource const &other) {
    mResourcePtr = other.mResourcePtr;
    mCounter = other.mCounter;
    return *this;
  }
  Resource &operator=(Resource &&other) {
    mResourcePtr = other.mResourcePtr;
    mCounter = other.mCounter;
    other.mResourcePtr = nullptr;
    return *this;
  }
  Resource &operator=(T *ptr) {
    if (mResourcePtr == ptr) {
      return *this;
    }
    if (mResourcePtr) {
      mCounter.Delete();
    }
    mResourcePtr = ptr;
    mCounter = SharedCount(ptr);
    return *this;
  }
  Resource &operator=(std::nullptr_t) {
    mResourcePtr = nullptr;
    mCounter.Delete();
    return *this;
  }

  operator bool() const { return mResourcePtr != nullptr; }

public:
  template <typename... Args> static Resource<T> Create(Args &&...args) {
    return Resource<T>(new T(std::forward<Args>(args)...));
  }
};

} // namespace Terreate::Core
