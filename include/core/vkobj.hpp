#pragma once
#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Core {
template <typename T> class VkObjectRef {
private:
  VkObjectRef &operator=(T *) = delete;
  VkObjectRef &operator=(T const *) = delete;

  template <typename U>
  friend class VkObject; // Allow VkObject to access private members

private:
  T *mVulkanObjectRef = nullptr;

private:
  T *get() const {
    if (mVulkanObjectRef == nullptr) {
      throw Exception::NullReferenceException(
          "Vulkan object reference is null. Ensure the object is initialized "
          "before accessing it.");
    }
    return mVulkanObjectRef;
  }

  VkObjectRef(T *vulkanObjectRef) : mVulkanObjectRef(vulkanObjectRef) {
    if (mVulkanObjectRef == nullptr) {
      throw Exception::NullReferenceException(
          "Vulkan object reference cannot be null.");
    }
  }

public:
  VkObjectRef() = default;
  VkObjectRef(std::nullptr_t) : mVulkanObjectRef(nullptr) {}
  VkObjectRef(VkObjectRef const &other)
      : mVulkanObjectRef(other.mVulkanObjectRef) {}
  virtual ~VkObjectRef() = default;

  void reset() { mVulkanObjectRef = nullptr; }

  T &operator*() { return *this->get(); }
  T const &operator*() const { return *this->get(); }

  T *operator->() { return this->get(); }
  T const *operator->() const { return this->get(); }

  VkObjectRef &operator=(std::nullptr_t) {
    mVulkanObjectRef = nullptr;
    return *this;
  }
  VkObjectRef &operator=(VkObjectRef const &other) {
    if (this != &other) {
      mVulkanObjectRef = other.mVulkanObjectRef;
    }
    return *this;
  }

  operator bool() const { return mVulkanObjectRef != nullptr; }
};

template <typename T> class VkObject {
private:
  PROHIBIT_COPY_AND_ASSIGN(VkObject);
  template <typename U>
  friend class VkObject; // Allow VkObjectRef to access private members

private:
  Type::unique<T> mVulkanObject = nullptr;

public:
  VkObject() = default;
  VkObject(std::nullptr_t) : mVulkanObject(nullptr) {}
  template <typename U>
  VkObject(U *vulkanObject) : mVulkanObject(vulkanObject) {
    if (mVulkanObject == nullptr) {
      throw Exception::NullReferenceException("Vulkan object cannot be null.");
    }
  }
  template <typename U>
  explicit VkObject(Type::unique<U> &&vulkanObject)
      : mVulkanObject(std::move(vulkanObject)) {}
  template <typename U>
  VkObject(VkObject<U> &&other) noexcept
      : mVulkanObject(std::move(other.mVulkanObject)) {}
  virtual ~VkObject() = default;

  VkObjectRef<T> ref() const {
    if (mVulkanObject == nullptr) {
      throw Exception::NullReferenceException(
          "Vulkan object is null. Ensure the object is initialized before "
          "accessing it.");
    }
    return VkObjectRef<T>(mVulkanObject.get());
  }
  T *get() { return mVulkanObject.get(); }
  T const *get() const { return mVulkanObject.get(); }

  void reset() { mVulkanObject.reset(); }
  void reset(T *vulkanObject) { mVulkanObject.reset(vulkanObject); }
  void dispose() {
    if (mVulkanObject) {
      mVulkanObject.reset();
    }
  }

  T &operator*() { return *mVulkanObject; }
  T const &operator*() const { return *mVulkanObject; }
  T *operator->() { return mVulkanObject.get(); }
  T const *operator->() const { return mVulkanObject.get(); }

  VkObject &operator=(std::nullptr_t) {
    mVulkanObject.reset();
    return *this;
  }
  template <typename U> VkObject &operator=(U *vulkanObject) {
    if (mVulkanObject.get() != vulkanObject) {
      mVulkanObject.reset(vulkanObject);
    }
    return *this;
  }
  template <typename U> VkObject &operator=(VkObject<U> &&other) noexcept {
    mVulkanObject = std::move(other.mVulkanObject);
    return *this;
  }

  operator bool() const { return mVulkanObject != nullptr; }

public:
  template <typename... Args> static VkObject create(Args &&...args) {
    return VkObject(new T(std::forward<Args>(args)...));
  }
};

template <typename T, typename... Args>
VkObject<T> makeVkObject(Args &&...args) {
  return VkObject<T>::create(std::forward<Args>(args)...);
}
} // namespace Terreate::Core
