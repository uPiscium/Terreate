#pragma once
#include "device.hpp"
#include "pipeline.hpp"

#include "../decl/type.hpp"
#include "../util/resourceptr.hpp"

namespace Terreate::Core {
class Framebuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(Framebuffer);

private:
  Util::ResourcePointer<Device> mDevice;
  Type::vec<VkFramebuffer> mFramebuffers;

public:
  Framebuffer(Util::ResourcePointer<Pipeline> pipeline);
  ~Framebuffer() { this->dispose(); }

  Util::ResourcePointer<Device> getDevice() const { return mDevice; }
  Type::vec<VkFramebuffer> const &getFramebuffers() const {
    return mFramebuffers;
  }

  void dispose();

  VkFramebuffer operator[](Type::u64 const &index) const {
    return mFramebuffers[index];
  }
};
} // namespace Terreate::Core
