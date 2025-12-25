#pragma once

#include "common/type.hpp"

#include "device.hpp"

namespace Terreate::Core::RenderPass {

class RenderPass {
private:
  PROHIBIT_COPY_AND_ASSIGN(RenderPass);

private:
  VkRenderPass mHandle = VK_NULL_HANDLE;

private:
  RenderPass(shared<Core::Vulkan::Device> const &device);

public:
  ~RenderPass();

  void addAttachment(VkAttachmentDescription const &description,
                     VkAttachmentReference const &reference);
};

class RenderPassBuilder {
private:
  shared<RenderPass> mHandle = nullptr;

public:
  RenderPassBuilder(shared<Core::Vulkan::Device> const &device);

  void addColorAttachment(VkFormat const &format, VkAttachmentLoadOp loadOp,
                          VkAttachmentStoreOp storeOp,
                          VkImageLayout initialLayout,
                          VkImageLayout finalLayout);
  void addDepthAttachment(VkFormat const &format, VkAttachmentLoadOp loadOp,
                          VkAttachmentStoreOp storeOp,
                          VkImageLayout initialLayout,
                          VkImageLayout finalLayout);
  void addStencilAttachment(VkFormat const &format, VkAttachmentLoadOp loadOp,
                            VkAttachmentStoreOp storeOp,
                            VkImageLayout initialLayout,
                            VkImageLayout finalLayout);
  void addDepthStencilAttachment(VkFormat const &format,
                                 VkAttachmentLoadOp loadOp,
                                 VkAttachmentStoreOp storeOp,
                                 VkImageLayout initialLayout,
                                 VkImageLayout finalLayout);
  void addResolveAttachment(VkFormat const &format, VkAttachmentLoadOp loadOp,
                            VkAttachmentStoreOp storeOp,
                            VkImageLayout initialLayout,
                            VkImageLayout finalLayout);

  void
  addSubpass(VkPipelineBindPoint const &pipelineBindPoint,
             vec<VkAttachmentReference> const &colorAttachments,
             VkAttachmentReference const *depthStencilAttachment = nullptr,
             vec<VkAttachmentReference> const *resolveAttachments = nullptr);
};
} // namespace Terreate::Core::RenderPass
