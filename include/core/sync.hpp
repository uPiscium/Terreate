#pragma once
#include "device.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class ISemaphore {
public:
  virtual ~ISemaphore() = default;
  virtual operator VkSemaphore() const = 0;
};

class IFence {
public:
  virtual ~IFence() = default;
  virtual bool isSignaled() const = 0;
  virtual void reset() = 0;
  virtual bool wait(uint64_t timeout = UINT64_MAX) = 0;
  virtual operator VkFence() const = 0;
};

class Semaphore : public ISemaphore {
private:
  VkObjectRef<Device> mDevice;
  VkSemaphore mSemaphore = VK_NULL_HANDLE;

public:
  Semaphore(VkObjectRef<Device> device);
  ~Semaphore() override;
  operator VkSemaphore() const override { return mSemaphore; }
};

class Fence : public IFence {
private:
  VkObjectRef<Device> mDevice;
  VkFence mFence = VK_NULL_HANDLE;

public:
  Fence(VkObjectRef<Device> device);
  ~Fence() override;

  bool isSignaled() const override {
    return vkGetFenceStatus(*mDevice, mFence) == VK_SUCCESS;
  }

  void reset() override;
  bool wait(uint64_t timeout = UINT64_MAX) override {
    return vkWaitForFences(*mDevice, 1, &mFence, VK_TRUE, timeout) ==
           VK_SUCCESS;
  }

  operator VkFence() const override { return mFence; }
};
} // namespace Terreate::Core
