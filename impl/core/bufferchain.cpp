#include "../../include/core/bufferchain.hpp"

namespace Terreate::Core {
DisplayBufferChain::~DisplayBufferChain() {
  mSwapchain.reset();
  mFramebuffer.reset();
}

void DisplayBufferChain::attachRenderPass(
    Vulkan::VkObjectRef<Vulkan::RenderPass> renderPass) {
  mRenderPass = renderPass;
  mFramebuffer = Vulkan::VkObject<Vulkan::Framebuffer>::create(
      mDevice, renderPass, mSwapchain.ref());
}
} // namespace Terreate::Core
