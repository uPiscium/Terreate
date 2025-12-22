#pragma once

#include "common/type.hpp"

#include "command.hpp"
#include "device.hpp"
#include "swapchain.hpp"

namespace Terreate::Core::Vulkan {

class Queue {
private:
  PROHIBIT_COPY_AND_ASSIGN(Queue);

private:
  shared<Device> mDevice = nullptr;
  VkQueue mHandle = VK_NULL_HANDLE;

private:
  Queue(shared<Device> const &device);

public:
  ~Queue() = default;

  void submit(vec<VkSemaphore> const &wait, vec<VkSemaphore> const &signal,
              vec<shared<CommandBuffer>> const &commandBuffer,
              VkFence fence = VK_NULL_HANDLE) const;
  void submit(vec<shared<CommandBuffer>> const &commandBuffer,
              VkFence fence = VK_NULL_HANDLE) const;
  // returns false if swapchain is suboptimal or out of date
  vec<bool> present(vec<shared<Swapchain>> swapchains, u32 imageIndex,
                    vec<VkSemaphore> const &wait) const;
  bool present(shared<Swapchain> const &swapchain, u32 imageIndex,
               vec<VkSemaphore> const &wait) const;
  void wait() const;

  operator VkQueue() const;

public:
  static shared<Queue> create(shared<Device> const &device);
};

} // namespace Terreate::Core::Vulkan
