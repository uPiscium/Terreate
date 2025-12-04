#pragma once

#include "common/type.hpp"

#include "device.hpp"
#include "enum.hpp"

namespace Terreate::Vulkan {

class CommandPool {
private:
  PROHIBIT_COPY_AND_ASSIGN(CommandPool);

private:
  shared<Device> mDevice = nullptr;
  VkCommandPool mHandle = VK_NULL_HANDLE;

private:
  CommandPool(shared<Device> const &device, bool transient, bool protect);

public:
  ~CommandPool();

  operator VkCommandPool() const;

public:
  static shared<CommandPool> create(shared<Device> const &device,
                                    bool transient = false,
                                    bool protect = false);
};

class CommandBuffer {
private:
  PROHIBIT_COPY_AND_ASSIGN(CommandBuffer);

private:
  shared<Device> mDevice = nullptr;
  shared<CommandPool> mCommandPool = nullptr;
  VkCommandBuffer mHandle = VK_NULL_HANDLE;

private:
  CommandBuffer(shared<Device> const &device,
                shared<CommandPool> const &commandPool,
                CommandBufferLevel level);

public:
  ~CommandBuffer();

  // void setViewport(Viewport const &viewport);
  // void setViewports(vec<Viewport> const &viewports, u32 firstViewport = 0);
  // void setScissor(Rect2D const &scissor);
  // void setScissors(vec<Rect2D> const &scissors, u32 firstScissor = 0);
  // void setLineWidth(float lineWidth);
  // void setDepthBias(float constant, float clamp, float slope);
  // void setBlendConstants(vec4 const &blendConstants);
  // void setDepthBounds(float minDepth, float maxDepth);
  // void setStencilCompareMask(FaceMask faceMask, u32 compareMask);
  // void setStencilWriteMask(FaceMask faceMask, u32 writeMask);
  // void setStencilReference(FaceMask faceMask, u32 reference);
  // void bind(shared<Pipeline> const &pipeline, PipelineBindPoint bindPoint);
  // void bind(shared<VkDescriptorSet> const &descriptorSet, PipelineBindPoint
  // bindPoint, shared<PipelineLayout> const &layout, vec<u32> const
  // &dynamicOffsets = {}, u32 firstSet = 0);
  // void bind(shared<Buffer> const &buffer, u64 offset, IndexType indexType);
  // void bind(shared<Buffer> const &buffer, u64 offset);
  // void bind(vec<shared<Buffer>> const &buffers, vec<u64> const &offsets,
  // u32 first = 0);
  // void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0,
  // u32 firstInstance = 0);
  // void drawIndexed(u32 indexCount, u32 instanceCount = 1,
  // u32 firstIndex = 0,
  // u32 vertexOffset = 0, u32 firstInstance = 0);
  // void drawIndirect(); // TODO
  // void drawIndexedIndirect(); // TODO
  void begin(CommandBufferUsage usage,
             shared<CommandBuffer> const &inheritFrom = nullptr);
  void end();
  void reset(bool releaseResources = false);

  operator VkCommandBuffer() const;

public:
  static shared<CommandBuffer>
  create(shared<Device> const &device, shared<CommandPool> const &commandPool,
         CommandBufferLevel level = CommandBufferLevel::PRIMARY);
};

} // namespace Terreate::Vulkan
