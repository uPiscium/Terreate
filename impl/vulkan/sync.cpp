#include "../../include/common/exception.hpp"
#include "../../include/vulkan/sync.hpp"

namespace Terreate::Vulkan {
Semaphore::Semaphore(VkObjectRef<IDevice> device) : mDevice(device) {
  VkSemaphoreCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if (vkCreateSemaphore(*mDevice, &createInfo, nullptr, &mSemaphore) !=
      VK_SUCCESS) {
    throw Exception::SyncObjectCreationFailure("failed to create semaphore!");
  }
}

Semaphore::~Semaphore() {
  if (mSemaphore != VK_NULL_HANDLE) {
    vkDestroySemaphore(*mDevice, mSemaphore, nullptr);
    mSemaphore = VK_NULL_HANDLE;
  }
}

Fence::Fence(VkObjectRef<IDevice> device) : mDevice(device) {
  VkFenceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  if (vkCreateFence(*mDevice, &createInfo, nullptr, &mFence) != VK_SUCCESS) {
    throw Exception::SyncObjectCreationFailure("failed to create fence!");
  }
}

void Fence::reset() {
  if (vkResetFences(*mDevice, 1, &mFence) != VK_SUCCESS) {
    throw Exception::SyncObjectResetFailure("failed to reset fence!");
  }
}

Fence::~Fence() {
  if (mFence != VK_NULL_HANDLE) {
    vkDestroyFence(*mDevice, mFence, nullptr);
    mFence = VK_NULL_HANDLE;
  }
}
} // namespace Terreate::Vulkan
