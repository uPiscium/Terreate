#pragma once
#include "device.hpp"
#include "swapchain.hpp"
#include "sync.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class Pipeline {
private:
  PROHIBIT_COPY_AND_ASSIGN(Pipeline);

private:
  VkObjectRef<Device> mDevice;
  VkObjectRef<Swapchain> mSwapchain;

  Type::vec<Type::byte> mVertShaderCode;
  Type::vec<Type::byte> mFragShaderCode;

  VkRenderPass mRenderPass = VK_NULL_HANDLE;
  VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;

private:
  VkShaderModule createShaderModule(Type::vec<Type::byte> const &code);
  void createRenderPass();
  void createPipeline();
  void compileShader(Type::str const &vert, Type::str const &frag);

public:
  Pipeline(VkObjectRef<Device> device, VkObjectRef<Swapchain> swapchain)
      : mDevice(device), mSwapchain(swapchain) {
    this->createRenderPass();
  }
  ~Pipeline() { this->dispose(); }

  VkObjectRef<Device> getDevice() const { return mDevice; }
  VkObjectRef<Swapchain> getSwapchain() const { return mSwapchain; }
  VkPipelineLayout getLayout() const { return mPipelineLayout; }
  Type::u32 getNextImageIndex(VkObjectRef<Semaphore> semaphore) const {
    return mSwapchain->getNextImageIndex(semaphore);
  }

  void attachCompiledShaderSources(Type::vec<Type::byte> const &vert,
                                   Type::vec<Type::byte> const &frag);
  void attachShaderSources(Type::str const &vert, Type::str const &frag) {
    this->compileShader(vert, frag);
  }

  void dispose();

  operator VkRenderPass() const { return mRenderPass; }
  operator VkPipeline() const { return mGraphicsPipeline; }
};
} // namespace Terreate::Core
