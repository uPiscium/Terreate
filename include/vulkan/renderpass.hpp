#pragma once
#include "device.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

namespace Terreate::Vulkan {
class RenderPass {
private:
  PROHIBIT_COPY_AND_ASSIGN(RenderPass);

private:
  VkObjectRef<Device> mDevice;
  VkObjectRef<Swapchain> mSwapchain;
  VkRenderPass mRenderPass = VK_NULL_HANDLE;

private:
  void createRenderPass();

public:
  RenderPass(VkObjectRef<Device> device, VkObjectRef<Swapchain> swapchain)
      : mDevice(device), mSwapchain(swapchain) {
    this->createRenderPass();
  }
  ~RenderPass();

  operator VkRenderPass() const { return mRenderPass; }
};
} // namespace Terreate::Vulkan
