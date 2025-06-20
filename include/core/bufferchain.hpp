#pragma once
#include "display.hpp"

#include "../vulkan/device.hpp"
#include "../vulkan/framebuffer.hpp"
#include "../vulkan/renderpass.hpp"
#include "../vulkan/swapchain.hpp"
#include "../vulkan/vkobj.hpp"

namespace Terreate::Core {
class IBufferChain {
public:
  virtual ~IBufferChain() = default;

  virtual Vulkan::VkObjectRef<Vulkan::Swapchain> getSwapchain() const = 0;
  virtual Vulkan::VkObjectRef<Vulkan::Framebuffer> getFramebuffer() const = 0;
  virtual Type::ImageFormat getImageFormat() const = 0;
  virtual void
  attachRenderPass(Vulkan::VkObjectRef<Vulkan::RenderPass> renderPass) = 0;
};

class DisplayBufferChain : public IBufferChain {
private:
  Vulkan::VkObjectRef<Vulkan::Device> mDevice;

  Vulkan::VkObject<Vulkan::Swapchain> mSwapchain;
  Vulkan::VkObjectRef<Vulkan::RenderPass> mRenderPass;
  Vulkan::VkObject<Vulkan::Framebuffer> mFramebuffer;

public:
  DisplayBufferChain(Vulkan::VkObjectRef<Vulkan::Device> device,
                     Vulkan::VkObject<IDisplay> display)
      : mDevice(device) {
    mSwapchain = Vulkan::VkObject<Vulkan::Swapchain>::create(
        device, display->getProperties().framebufferSize,
        display->getSurface());
  }
  ~DisplayBufferChain() override;

  Vulkan::VkObjectRef<Vulkan::Swapchain> getSwapchain() const override {
    return mSwapchain.ref();
  }
  Vulkan::VkObjectRef<Vulkan::Framebuffer> getFramebuffer() const override {
    return mFramebuffer.ref();
  }
  Type::ImageFormat getImageFormat() const override {
    return mSwapchain->getProperty().imageFormat;
  }
  void
  attachRenderPass(Vulkan::VkObjectRef<Vulkan::RenderPass> renderPass) override;
};
} // namespace Terreate::Core
