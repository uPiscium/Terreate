#pragma once
#include "device.hpp"
#include "renderpass.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class IFramebuffer {
public:
  virtual ~IFramebuffer() = default;
  virtual Type::vec<VkFramebuffer> const &getFramebuffers() const = 0;
  virtual VkFramebuffer operator[](Type::u64 const &index) const = 0;
};

class Framebuffer : public IFramebuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(Framebuffer);

private:
  VkObjectRef<Device> mDevice;
  Type::vec<VkFramebuffer> mFramebuffers;

public:
  Framebuffer(VkObjectRef<Device> device, VkObjectRef<IRenderPass> renderPass,
              VkObjectRef<Swapchain> swapchain);
  ~Framebuffer() override;

  Type::vec<VkFramebuffer> const &getFramebuffers() const override {
    return mFramebuffers;
  }

  VkFramebuffer operator[](Type::u64 const &index) const override {
    return mFramebuffers[index];
  }
};
} // namespace Terreate::Core
