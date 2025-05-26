#include "../../include/core/framebuffer.hpp"
#include "../../include/decl/exception.hpp"

namespace Terreate::Core {
Framebuffer::Framebuffer(Util::ResourcePointer<Pipeline> pipeline)
    : mDevice(pipeline->getDevice()) {
  auto const &extent = pipeline->getSwapchain()->getProperty().extent;
  auto const &imageViews = pipeline->getSwapchain()->getImageViews();
  mFramebuffers.resize(imageViews.size());
  for (size_t i = 0; i < imageViews.size(); ++i) {
    VkImageView attachments[] = {imageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = *pipeline;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(*mDevice, &framebufferInfo, nullptr,
                            &mFramebuffers[i]) != VK_SUCCESS) {
      throw Exception::FramebufferCreationFailure(
          "Failed to create framebuffer for swapchain image view.");
    }
  }
}

void Framebuffer::dispose() {
  for (auto framebuffer : mFramebuffers) {
    vkDestroyFramebuffer(*mDevice, framebuffer, nullptr);
  }
  mFramebuffers.clear();
}
} // namespace Terreate::Core
