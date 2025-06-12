#pragma once
#include "device.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

namespace Terreate::Vulkan {
class IRenderPass {
public:
  virtual ~IRenderPass() = default;
  virtual operator VkRenderPass() const = 0;
};

class RenderPass : public IRenderPass {
private:
  PROHIBIT_COPY_AND_ASSIGN(RenderPass);

private:
  VkObjectRef<IDevice> mDevice;
  VkObjectRef<ISwapchain> mSwapchain;
  VkRenderPass mRenderPass = VK_NULL_HANDLE;

private:
  void createRenderPass();

public:
  RenderPass(VkObjectRef<IDevice> device, VkObjectRef<ISwapchain> swapchain)
      : mDevice(device), mSwapchain(swapchain) {
    this->createRenderPass();
  }
  ~RenderPass() override;

  operator VkRenderPass() const override { return mRenderPass; }
};
} // namespace Terreate::Vulkan
