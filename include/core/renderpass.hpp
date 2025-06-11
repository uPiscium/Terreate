#pragma once
#include "device.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

namespace Terreate::Core {
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
  RenderPass(VkObjectRef<Swapchain> swapchain)
      : mDevice(swapchain->getDevice()), mSwapchain(swapchain) {
    this->createRenderPass();
  }
  ~RenderPass() { this->dispose(); }

  VkRenderPass getRenderPass() const { return mRenderPass; }

  void dispose();

  operator VkRenderPass() const { return mRenderPass; }
};
} // namespace Terreate::Core
