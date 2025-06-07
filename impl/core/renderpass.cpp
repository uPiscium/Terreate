#include "../../include/core/renderpass.hpp"

namespace Terreate::Core {
VkAttachmentDescription RenderPass::getAttachment() const {
  VkAttachmentDescription colorAttachment = {};
  colorAttachment.format = mSwapchain->getProperty().imageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  return colorAttachment;
}

VkAttachmentReference RenderPass::getAttachmentRef() const {
  VkAttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment = 0; // Index of the attachment
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  return colorAttachmentRef;
}

VkSubpassDescription RenderPass::getSubpassDescription(
    Type::vec<VkAttachmentReference> const &refs) const {
  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = static_cast<Type::u32>(refs.size());
  subpass.pColorAttachments = refs.data();
  return subpass;
}

VkSubpassDependency RenderPass::getSubpassDependency() const {
  VkSubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  return dependency;
}

VkRenderPassCreateInfo RenderPass::getRenderPassInfo(
    Type::vec<VkAttachmentDescription> const &attachments,
    Type::vec<VkSubpassDescription> const &subpasses,
    Type::vec<VkSubpassDependency> const &dependencies) const {
  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<Type::u32>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = static_cast<Type::u32>(subpasses.size());
  renderPassInfo.pSubpasses = subpasses.data();
  renderPassInfo.dependencyCount = static_cast<Type::u32>(dependencies.size());
  renderPassInfo.pDependencies = dependencies.data();
  return renderPassInfo;
}

void RenderPass::createRenderPass() {
  VkAttachmentDescription colorAttachment = this->getAttachment();
  VkAttachmentReference colorAttachmentRef = this->getAttachmentRef();
  VkSubpassDescription subpass =
      this->getSubpassDescription({colorAttachmentRef});
  VkSubpassDependency dependency = this->getSubpassDependency();
  Type::vec<VkAttachmentDescription> attachments = {colorAttachment};
  Type::vec<VkSubpassDescription> subpasses = {subpass};
  VkRenderPassCreateInfo renderPassInfo =
      this->getRenderPassInfo(attachments, subpasses, {dependency});
  std::cout << "Creating render pass..." << std::endl;

  if (vkCreateRenderPass(*mDevice, &renderPassInfo, nullptr, &mRenderPass) !=
      VK_SUCCESS) {
    throw Exception::RenderPassCreationFailure("Failed to create render pass.");
  }
}

void RenderPass::dispose() {
  if (mRenderPass != VK_NULL_HANDLE) {
    vkDestroyRenderPass(*mDevice, mRenderPass, nullptr);
    mRenderPass = VK_NULL_HANDLE;
  }
}
} // namespace Terreate::Core
