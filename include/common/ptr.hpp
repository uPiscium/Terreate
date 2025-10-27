#pragma once
#include "common/type.hpp"

namespace Terreate {

class IDisposable {
public:
  virtual ~IDisposable() = default;
  virtual void dispose() = 0;
};

template <typename T> class CompositeDisposable : public IDisposable {
protected:
  shared<T> mHandle = nullptr;
  vec<shared<IDisposable>> mChildObjects;

public:
  CompositeDisposable() {}
  explicit CompositeDisposable(shared<T> handle) : mHandle(handle) {}
  explicit CompositeDisposable(T *handle) : mHandle(shared<T>(handle)) {}
  template <typename Deleter>
  explicit CompositeDisposable(T *handle, Deleter deleter)
      : mHandle(shared<T>(handle, deleter)) {}
  explicit CompositeDisposable(const CompositeDisposable<T> &other)
      : mHandle(other.mHandle), mChildObjects(other.mChildObjects) {}
  explicit CompositeDisposable(CompositeDisposable<T> &&other) noexcept
      : mHandle(std::move(other.mHandle)),
        mChildObjects(std::move(other.mChildObjects)) {}
  virtual ~CompositeDisposable() { this->dispose(); }

  shared<T> getHandle() const { return mHandle; }
  T *get() { return mHandle.get(); }
  T const *get() const { return mHandle.get(); }

  void dispose() override;

  T *operator->() { return mHandle.get(); }
  T const *operator->() const { return mHandle.get(); }
  T &operator*() { return *mHandle; }
  T const &operator*() const { return *mHandle; }

  operator bool() const { return mHandle != nullptr; }
  operator shared<T>() const { return mHandle; }
  operator T *() { return mHandle.get(); }
  operator T const *() const { return mHandle.get(); }
};

} // namespace Terreate

// ===================================================================
// Implementation
// ===================================================================
namespace Terreate {

template <typename T> void CompositeDisposable<T>::dispose() {
  for (auto &disposable : mChildObjects) {
    disposable->dispose();
  }
  mChildObjects.clear();
  mHandle = nullptr;
}

} // namespace Terreate
