#pragma once
#include "device.hpp"
#include "renderpass.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class IPipeline {
public:
  virtual ~IPipeline() = default;
  virtual VkPipelineLayout getLayout() const = 0;
  virtual void
  attachCompiledShaderSources(Type::vec<Type::byte> const &vert,
                              Type::vec<Type::byte> const &frag) = 0;
  virtual void attachShaderSources(Type::str const &vert,
                                   Type::str const &frag) = 0;
  virtual operator VkPipeline() const = 0;
};

class Pipeline : public IPipeline {
private:
  PROHIBIT_COPY_AND_ASSIGN(Pipeline);

private:
  VkObjectRef<IDevice> mDevice;
  VkObjectRef<IRenderPass> mRenderPass;

  Type::vec<Type::byte> mVertShaderCode;
  Type::vec<Type::byte> mFragShaderCode;

  VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;

private:
  VkShaderModule createShaderModule(Type::vec<Type::byte> const &code);
  void createPipeline();
  void compileShader(Type::str const &vert, Type::str const &frag);

public:
  Pipeline(VkObjectRef<IDevice> device, VkObjectRef<IRenderPass> renderPass)
      : mDevice(device), mRenderPass(renderPass) {}
  ~Pipeline();

  VkPipelineLayout getLayout() const override { return mPipelineLayout; }

  void attachCompiledShaderSources(Type::vec<Type::byte> const &vert,
                                   Type::vec<Type::byte> const &frag) override;
  void attachShaderSources(Type::str const &vert,
                           Type::str const &frag) override {
    this->compileShader(vert, frag);
  }

  operator VkPipeline() const override { return mGraphicsPipeline; }
};
} // namespace Terreate::Core
