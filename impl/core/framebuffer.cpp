#include "../../include/common/exception.hpp"
#include "../../include/core/framebuffer.hpp"

namespace Terreate::Core {
Framebuffer::Framebuffer(VkObjectRef<Device> device,
                         VkObjectRef<RenderPass> renderPass,
                         VkObjectRef<Swapchain> swapchain)
    : mDevice(device) {
  auto const &extent = swapchain->getProperty().extent;
  auto const &imageViews = swapchain->getImageViews();
  mFramebuffers.resize(imageViews.size());
  for (size_t i = 0; i < imageViews.size(); ++i) {
    VkImageView attachments[] = {imageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = *renderPass;
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

Framebuffer::~Framebuffer() {
  for (auto framebuffer : mFramebuffers) {
    vkDestroyFramebuffer(*mDevice, framebuffer, nullptr);
  }
  mFramebuffers.clear();
}
} // namespace Terreate::Core
