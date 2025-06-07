#pragma once
#include "device.hpp"
#include "swapchain.hpp"

namespace Terreate::Core {
class IRenderPass {
public:
  virtual ~IRenderPass() = default;
  virtual operator VkRenderPass() const = 0;
};

class RenderPass : public IRenderPass {
private:
  VkObjectRef<IDevice> mDevice;
  VkObjectRef<ISwapchain> mSwapchain;
  VkRenderPass mRenderPass = VK_NULL_HANDLE;

private:
  VkAttachmentDescription getAttachment() const;
  VkAttachmentReference getAttachmentRef() const;
  VkSubpassDescription
  getSubpassDescription(Type::vec<VkAttachmentReference> const &refs) const;
  VkSubpassDependency getSubpassDependency() const;
  VkRenderPassCreateInfo getRenderPassInfo(
      Type::vec<VkAttachmentDescription> const &attachments = {},
      Type::vec<VkSubpassDescription> const &subpasses = {},
      Type::vec<VkSubpassDependency> const &dependencies = {}) const;
  void createRenderPass();

public:
  RenderPass(VkObjectRef<IDevice> device, VkObjectRef<ISwapchain> swapchain)
      : mDevice(device), mSwapchain(swapchain) {
    this->createRenderPass();
  }
  ~RenderPass() override { this->dispose(); }

  void dispose();

  operator VkRenderPass() const override { return mRenderPass; }
};
} // namespace Terreate::Core
