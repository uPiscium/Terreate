#pragma once
#include "device.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Vulkan {
class Semaphore {
private:
  VkObjectRef<Device> mDevice;
  VkSemaphore mSemaphore = VK_NULL_HANDLE;

public:
  Semaphore(VkObjectRef<Device> device);
  ~Semaphore();
  operator VkSemaphore() const { return mSemaphore; }
};

class Fence {
private:
  VkObjectRef<Device> mDevice;
  VkFence mFence = VK_NULL_HANDLE;

public:
  Fence(VkObjectRef<Device> device);
  ~Fence();

  bool isSignaled() const {
    return vkGetFenceStatus(*mDevice, mFence) == VK_SUCCESS;
  }

  void reset();
  bool wait(uint64_t timeout = UINT64_MAX) {
    return vkWaitForFences(*mDevice, 1, &mFence, VK_TRUE, timeout) ==
           VK_SUCCESS;
  }

  operator VkFence() const { return mFence; }
};
} // namespace Terreate::Vulkan
