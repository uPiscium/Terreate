#pragma once
#include "device.hpp"
#include "pipeline.hpp"
#include "renderpass.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"

#include "../common/enum.hpp"
#include "../common/type.hpp"
#include "../util/bitflag.hpp"

namespace Terreate::Core {
class ICommandBuffer {
public:
  virtual ~ICommandBuffer() = default;

  virtual VkObjectRef<Device> getDevice() const = 0;

  virtual void setRenderPass(VkObjectRef<RenderPass> renderPass,
                             VkObjectRef<Swapchain> swapchain,
                             VkObjectRef<IPipeline> pipeline,
                             VkFramebuffer framebuffer,
                             Type::vec<float> const &clearColor,
                             Type::SubpassContent content) = 0;
  virtual void setViewport(float x0, float y0, float width, float height,
                           float minDepth, float maxDepth) = 0;
  virtual void setScissor(Type::i32 offsetX, Type::i32 offsetY, Type::u32 width,
                          Type::u32 height) = 0;

  virtual void begin(Bitflag<Type::CommandBufferUsage> flags = 0,
                     VkCommandBufferInheritanceInfo *inherit = nullptr) = 0;
  virtual void end() = 0;
  virtual void drawBuffer(Type::u64 verexCount, Type::u64 instanceCount,
                          Type::u64 firstVertex = 0,
                          Type::u64 firstInstance = 0) = 0;
  virtual void reset() = 0;

  virtual operator VkCommandBuffer() const = 0;
};

class ICommandPool {
public:
  virtual ~ICommandPool() = default;

  virtual VkObjectRef<ICommandBuffer>
  createCommandBuffer(Type::CommandBufferLevel const &level =
                          Type::CommandBufferLevel::PRIMARY) = 0;

  virtual operator VkCommandPool() const = 0;
};

class CommandPool : public ICommandPool {
private:
  PROHIBIT_COPY_AND_ASSIGN(CommandPool);

private:
  VkObjectRef<Device> mDevice;

  VkCommandPool mCommandPool = VK_NULL_HANDLE;
  Type::vec<VkObject<ICommandBuffer>> mCommandBuffers;

public:
  CommandPool(VkObjectRef<Device> device);
  ~CommandPool() override;

  VkObjectRef<ICommandBuffer>
  createCommandBuffer(Type::CommandBufferLevel const &level =
                          Type::CommandBufferLevel::PRIMARY) override;

  operator VkCommandPool() const override { return mCommandPool; }
};

class CommandBuffer : public ICommandBuffer {
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
  ~CommandBuffer() override;

  VkObjectRef<Device> getDevice() const override { return mDevice; }

  void setRenderPass(VkObjectRef<RenderPass> renderPass,
                     VkObjectRef<Swapchain> swapchain,
                     VkObjectRef<IPipeline> pipeline, VkFramebuffer framebuffer,
                     Type::vec<float> const &clearColor,
                     Type::SubpassContent content) override;
  void setViewport(float x0, float y0, float width, float height,
                   float minDepth, float maxDepth) override;
  void setScissor(Type::i32 offsetX, Type::i32 offsetY, Type::u32 width,
                  Type::u32 height) override;
  void begin(Bitflag<Type::CommandBufferUsage> flags = 0,
             VkCommandBufferInheritanceInfo *inherit = nullptr) override;
  void end() override;
  void drawBuffer(Type::u64 verexCount, Type::u64 instanceCount,
                  Type::u64 firstVertex = 0,
                  Type::u64 firstInstance = 0) override;
  void reset() override;

  operator VkCommandBuffer() const override { return mCommandBuffer; }
  // operator VkCommandBufferInheritanceInfo() const { return ; }
};
} // namespace Terreate::Core
