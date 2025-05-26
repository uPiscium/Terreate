#pragma once
#include "device.hpp"
#include "framebuffer.hpp"
#include "pipeline.hpp"

#include "../decl/enum.hpp"
#include "../decl/type.hpp"
#include "../util/bitflag.hpp"
#include "../util/resourceptr.hpp"
#include "vulkan/vulkan_core.h"

namespace Terreate::Core {
class CommandBuffer;

class CommandPool {
private:
  PROHIBIT_COPY_AND_ASSIGN(CommandPool);

private:
  Util::ResourcePointer<Pipeline> mPipeline;
  Util::ResourcePointer<Device> mDevice;

  VkCommandPool mCommandPool = VK_NULL_HANDLE;
  Type::vec<Util::ResourcePointerOwner<CommandBuffer>> mCommandBuffers;

public:
  CommandPool(Util::ResourcePointer<Pipeline> pipeline);
  ~CommandPool() { this->dispose(); }

  Util::ResourcePointer<Device> getDevice() const { return mDevice; }

  Util::ResourcePointer<CommandBuffer>
  createCommandBuffer(Type::CommandBufferLevel const &level =
                          Type::CommandBufferLevel::PRIMARY);
  void dispose();

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
  Util::ResourcePointer<Pipeline> mPipeline;
  Util::ResourcePointer<Device> mDevice;
  VkCommandPool mCommandPool;

  bool mIsRecording = false;
  VkCommandBuffer mCommandBuffer = VK_NULL_HANDLE;

  VkRenderPassBeginInfo mRenderPassInfo;
  VkSubpassContents mContent;
  VkViewport mViewport;
  VkRect2D mScissor;
  DrawSetting mDrawSetting;

private:
  void begin(Bitflag<Type::CommandBufferUsage> flags,
             VkCommandBufferInheritanceInfo *inherit = nullptr);
  void end();

public:
  CommandBuffer(Util::ResourcePointer<Pipeline> pipeline, VkCommandPool pool,
                Type::CommandBufferLevel const &level =
                    Type::CommandBufferLevel::PRIMARY);
  ~CommandBuffer() { this->dispose(); }

  Util::ResourcePointer<Device> getDevice() const { return mDevice; }

  void setRenderPass(VkFramebuffer framebuffer,
                     Type::vec<float> const &clearColor,
                     Type::SubpassContent content);
  void setViewport(float x0, float y0, float width, float height,
                   float minDepth, float maxDepth);
  void setScissor(Type::i32 offsetX, Type::i32 offsetY, Type::u32 width,
                  Type::u32 height);
  void drawBuffer(Type::u64 verexCount, Type::u64 instanceCount,
                  Type::u64 firstVertex = 0, Type::u64 firstInstance = 0);
  void compile(Bitflag<Type::CommandBufferUsage> flags,
               VkCommandBufferInheritanceInfo *inherit = nullptr);
  void dispose();

  operator VkCommandBuffer() const { return mCommandBuffer; }
  // operator VkCommandBufferInheritanceInfo() const { return ; }
};
} // namespace Terreate::Core
