#pragma once

#include <api.hpp>
#include <device.hpp>
#include <exception.hpp>
#include <hubptr.hpp>
#include <type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;
using namespace Terreate::Util::Pointer;

class Renderer {
private:
  RefPointer<LogicalDevice> mDevice;
  VkRenderPass mRenderPass;
  VkPipelineLayout mPipelineLayout;
  VkPipeline mGraphicsPipeline;

private:
  void createRenderpass(RefPointer<LogicalDevice> device, VkFormat format);

public:
  Renderer(RefPointer<LogicalDevice> device, VkFormat format);
  void load(vec<i8> const &vertcode, vec<i8> const &fragcode);
};
} // namespace Terreate::Core
