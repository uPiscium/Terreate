#pragma once
#include "device.hpp"
#include "pipeline.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class Framebuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(Framebuffer);

private:
  VkObjectRef<Device> mDevice;
  Type::vec<VkFramebuffer> mFramebuffers;

public:
  Framebuffer(VkObjectRef<Pipeline> pipeline);
  ~Framebuffer() { this->dispose(); }

  VkObjectRef<Device> getDevice() const { return mDevice; }
  Type::vec<VkFramebuffer> const &getFramebuffers() const {
    return mFramebuffers;
  }

  void dispose();

  VkFramebuffer operator[](Type::u64 const &index) const {
    return mFramebuffers[index];
  }
};
} // namespace Terreate::Core
