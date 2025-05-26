#pragma once
#include "../decl/exception.hpp"
#include "../decl/type.hpp"

namespace Terreate::Util {
template <typename T> class ResourcePointer {
private:
  ResourcePointer &operator=(T *) = delete;
  ResourcePointer &operator=(T const *) = delete;
  ResourcePointer &operator=(T &) = delete;
  ResourcePointer &operator=(T const &) = delete;

private:
  T **mResource = nullptr;

private:
  template <typename U, typename Deleter> friend class ResourcePointerOwner;

private:
  ResourcePointer(T **pointer) : mResource(pointer) {
    if (!mResource) {
      Type::str msg =
          "Resource type: " + Type::getTypeName<T>() + " pointer is null";
      throw Exception::NullReferenceException(msg);
    }
  }

  T *get() const {
    if (*mResource) {
      return *mResource;
    }
    Type::str msg =
        "Resource type: " + Type::getTypeName<T>() + " pointer is null";
    throw Exception::NullReferenceException(msg);
  }

public:
  ResourcePointer() : mResource(nullptr) {}
  ResourcePointer(std::nullptr_t) : mResource(nullptr) {}
  ResourcePointer(ResourcePointer const &other) { mResource = other.mResource; }
  ResourcePointer(ResourcePointer &&other) {
    mResource = other.mResource;
    other.mResource = nullptr;
  }
  ~ResourcePointer() { mResource = nullptr; }

  T *operator->() const { return this->get(); }
  T &operator*() { return *(this->get()); }
  T const &operator*() const { return *(this->get()); }

  ResourcePointer &operator=(ResourcePointer const &other) {
    if (this != &other) {
      mResource = other.mResource;
    }
    return *this;
  }
  ResourcePointer &operator=(ResourcePointer &&other) {
    if (this != &other) {
      mResource = other.mResource;
      other.mResource = nullptr;
    }
    return *this;
  }

  operator bool() const { return *mResource != nullptr; }
};

template <typename T, typename Deleter = std::default_delete<T>>
class ResourcePointerOwner {
private:
  ResourcePointerOwner(ResourcePointerOwner const &) = delete;
  ResourcePointerOwner &operator=(ResourcePointerOwner const &) = delete;

private:
  T *mPointer = nullptr;
  T **mResource = nullptr;
  Deleter mDeleter;

private:
  T *get(int) const {
    if (*mResource) {
      return *mResource;
    }
    Type::str msg =
        "Resource type: " + Type::getTypeName<T>() + " pointer is null";
    throw Exception::NullReferenceException(msg);
  }

public:
  ResourcePointerOwner() : mPointer(nullptr), mResource(nullptr) {}
  ResourcePointerOwner(std::nullptr_t)
      : mPointer(nullptr), mResource(nullptr) {}
  ResourcePointerOwner(T *pointer, Deleter deleter = Deleter())
      : mPointer(pointer), mDeleter(deleter) {
    mResource = &mPointer;
  }
  ResourcePointerOwner(ResourcePointerOwner &&other)
      : mPointer(other.mPointer) {
    mResource = &mPointer;
    mDeleter = std::move(other.mDeleter);
    *other.mResource = nullptr;
    other.mResource = nullptr;
  }
  ~ResourcePointerOwner() { this->dispose(); }

  ResourcePointer<T> get() const {
    if (mResource && *mResource) {
      return ResourcePointer<T>(mResource);
    }
    Type::str msg =
        "Resource type: " + Type::getTypeName<T>() + " pointer is null";
    throw Exception::NullReferenceException(msg);
  }

  void dispose() {
    if (!mResource) {
      return;
    }

    if (*mResource) {
      mDeleter(*mResource);
      *mResource = nullptr;
    }
    mResource = nullptr;
  }

  T *operator->() const { return this->get(0); }
  T &operator*() { return *(this->get(0)); }
  T const &operator*() const { return *(this->get(0)); }

  ResourcePointerOwner &operator=(ResourcePointerOwner &&other) {
    if (this == &other) {
      return *this;
    }

    this->dispose();
    mPointer = other.mPointer;
    mResource = &mPointer;
    mDeleter = std::move(other.mDeleter);
    *other.mResource = nullptr;
    other.mResource = nullptr;
    return *this;
  }
  ResourcePointerOwner &operator=(std::nullptr_t) {
    this->dispose();
    return *this;
  }

  operator bool() const {
    return mResource != nullptr && *mResource != nullptr;
  }

public:
  template <typename... Args>
  static ResourcePointerOwner<T> create(Args &&...args) {
    return ResourcePointerOwner<T>(new T(std::forward<Args>(args)...));
  }
};

template <typename T, typename... Args>
ResourcePointerOwner<T> createResource(Args &&...args) {
  return ResourcePointerOwner<T>::create(std::forward<Args>(args)...);
}
} // namespace Terreate::Util
