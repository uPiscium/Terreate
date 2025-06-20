#pragma once
#include "device.hpp"
#include "renderpass.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Vulkan {
class Framebuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(Framebuffer);

private:
  VkObjectRef<Device> mDevice;
  Type::vec<VkFramebuffer> mFramebuffers;

public:
  Framebuffer(VkObjectRef<Device> device, VkObjectRef<RenderPass> renderPass,
              VkObjectRef<Swapchain> swapchain);
  Framebuffer(VkObjectRef<Device> device,
              Type::vec<VkImageView> const &imageViews,
              VkExtent2D const &extent, VkObjectRef<RenderPass> renderPass);
  ~Framebuffer();

  Type::vec<VkFramebuffer> const &getFramebuffers() const {
    return mFramebuffers;
  }

  VkFramebuffer operator[](Type::u64 const &index) const {
    return mFramebuffers[index];
  }
};
} // namespace Terreate::Vulkan
