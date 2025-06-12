#pragma once
#include "device.hpp"
#include "pipeline.hpp"
#include "renderpass.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

#include "../common/enum.hpp"
#include "../common/type.hpp"
#include "../util/bitflag.hpp"

namespace Terreate::Vulkan {
class CommandBuffer;

class CommandPool {
private:
  PROHIBIT_COPY_AND_ASSIGN(CommandPool);

private:
  VkObjectRef<Device> mDevice;

  VkCommandPool mCommandPool = VK_NULL_HANDLE;
  Type::vec<VkObject<CommandBuffer>> mCommandBuffers;

public:
  CommandPool(VkObjectRef<Device> device);
  ~CommandPool();

  VkObjectRef<CommandBuffer>
  createCommandBuffer(Type::CommandBufferLevel const &level =
                          Type::CommandBufferLevel::PRIMARY);

  operator VkCommandPool() const { return mCommandPool; }
};

class CommandBuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(CommandBuffer);

private:
  struct DrawSetting {
    Type::u64 verexCount;
    Type::u64 instanceCount;
    Type::u64 firstVertex = 0;
    Type::u64 firstInstance = 0;
  };

private:
  VkObjectRef<Device> mDevice;
  VkCommandPool mCommandPool;

  bool mIsRecording = false;
  VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;

  VkRenderPassBeginInfo mRenderPassInfo;
  VkSubpassContents mContent;
  VkViewport mViewport;
  VkRect2D mScissor;
  DrawSetting mDrawSetting;

private:
public:
  CommandBuffer(VkObjectRef<Device> device, VkCommandPool pool,
                Type::CommandBufferLevel const &level =
                    Type::CommandBufferLevel::PRIMARY);
  ~CommandBuffer();

  void setRenderPass(VkObjectRef<RenderPass> renderPass,
                     VkObjectRef<Swapchain> swapchain,
                     VkObjectRef<Pipeline> pipeline, VkFramebuffer framebuffer,
                     Type::vec<float> const &clearColor,
                     Type::SubpassContent content);
  void setViewport(float x0, float y0, float width, float height,
                   float minDepth, float maxDepth);
  void setScissor(Type::i32 offsetX, Type::i32 offsetY, Type::u32 width,
                  Type::u32 height);
  void begin(Util::Bitflag<Type::CommandBufferUsage> flags = 0,
             VkCommandBufferInheritanceInfo *inherit = nullptr);
  void end();
  void drawBuffer(Type::u64 verexCount, Type::u64 instanceCount,
                  Type::u64 firstVertex = 0, Type::u64 firstInstance = 0);
  void reset();

  operator VkCommandBuffer() const { return mCommandBuffer; }
};
} // namespace Terreate::Vulkan
