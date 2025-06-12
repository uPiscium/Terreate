#pragma once
#include "device.hpp"
#include "renderpass.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Vulkan {
class Pipeline {
private:
  PROHIBIT_COPY_AND_ASSIGN(Pipeline);

private:
  VkObjectRef<Device> mDevice;
  VkObjectRef<RenderPass> mRenderPass;

  Type::vec<Type::byte> mVertShaderCode;
  Type::vec<Type::byte> mFragShaderCode;

  VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;

private:
  VkShaderModule createShaderModule(Type::vec<Type::byte> const &code);
  void createPipeline();
  void compileShader(Type::str const &vert, Type::str const &frag);

public:
  Pipeline(VkObjectRef<Device> device, VkObjectRef<RenderPass> renderPass)
      : mDevice(device), mRenderPass(renderPass) {}
  ~Pipeline();

  VkPipelineLayout getLayout() const { return mPipelineLayout; }

  void attachCompiledShaderSources(Type::vec<Type::byte> const &vert,
                                   Type::vec<Type::byte> const &frag);
  void attachShaderSources(Type::str const &vert, Type::str const &frag) {
    this->compileShader(vert, frag);
  }

  operator VkPipeline() const { return mGraphicsPipeline; }
};
} // namespace Terreate::Vulkan
