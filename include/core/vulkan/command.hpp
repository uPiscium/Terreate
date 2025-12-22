#pragma once

#include "common/type.hpp"

#include "device.hpp"
#include "enum.hpp"
#include "image.hpp"

namespace Terreate::Core::Vulkan {

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
  operator VkCommandBuffer() const;

public:
  static shared<CommandBuffer>
  create(shared<Device> const &device, shared<CommandPool> const &commandPool,
         CommandBufferLevel level = CommandBufferLevel::PRIMARY);
};

class CommandBufferEncoder {
private:
  PROHIBIT_COPY_AND_ASSIGN(CommandBufferEncoder);

private:
  shared<CommandBuffer> mCommandBuffer = nullptr;
  bool mIsEncoding = false;

private:
  CommandBufferEncoder(shared<CommandBuffer> const &commandBuffer);

public:
  ~CommandBufferEncoder();

  shared<CommandBuffer> const &getCommandBuffer() const;

  void setViewport(Viewport const &viewport);
  void setViewports(vec<Viewport> const &viewports, u32 firstViewport = 0);
  void setScissor(Rect2D const &scissor);
  void setScissors(vec<Rect2D> const &scissors, u32 firstScissor = 0);
  void setLineWidth(float lineWidth);
  void setDepthBias(float constant, float clamp, float slope);
  void setBlendConstants(vec4 const &blendConstants);
  void setDepthBounds(float minDepth, float maxDepth);
  void setStencilCompareMask(FaceMask faceMask, u32 compareMask);
  void setStencilWriteMask(FaceMask faceMask, u32 writeMask);
  void setStencilReference(FaceMask faceMask, u32 reference);

  bool isEncoding() const;

  // void bind(shared<Pipeline> const &pipeline, PipelineBindPoint bindPoint);
  // void bind(shared<VkDescriptorSet> const &descriptorSet,
  //           PipelineBindPoint bindPoint, shared<PipelineLayout> const
  //           &layout, vec<u32> const &dynamicOffsets = {}, u32 firstSet = 0);
  // void bind(shared<Buffer> const &buffer, u64 offset, IndexType indexType);
  // void bind(shared<Buffer> const &buffer, u64 offset);
  // void bind(vec<shared<Buffer>> const &buffers, vec<u64> const &offsets,
  //           u32 first = 0);
  void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0,
            u32 firstInstance = 0);
  void drawIndexed(u32 indexCount, u32 instanceCount = 1, u32 firstIndex = 0,
                   i32 vertexOffset = 0, u32 firstInstance = 0);
  // void drawIndirect(); // TODO
  // void drawIndexedIndirect(); // TODO
  void dispatch(u32 groupCountX, u32 groupCountY, u32 groupCountZ);
  // void dispatchIndirect(); // TODO
  // void copyBuffer(shared<Buffer> const &src,
  //                 shared<Buffer> const &dst,
  //                 vec<VkBufferCopy> const &regions);
  void copyImage(shared<Image> const &src, VkImageLayout const &srcLayout,
                 shared<Image> const &dst, VkImageLayout const &dstLayout,
                 vec<VkImageCopy> const &regions);
  void blitImage(shared<Image> const &src, VkImageLayout const &srcLayout,
                 shared<Image> const &dst, VkImageLayout const &dstLayout,
                 vec<VkImageBlit> const &regions, VkFilter filter);
  // void copyBufferToImage(shared<Buffer> const &src, VkImageLayout const
  // &srcLayout,
  //                        shared<Image> const &dst, VkImageLayout const
  //                        &dstLayout, vec<VkBufferImageCopy> const &regions);
  // void copyImageToBuffer(shared<Image> const &src, VkImageLayout const
  // &srcLayout,
  //                        shared<Buffer> const &dst, VkImageLayout const
  //                        &dstLayout, vec<VkBufferImageCopy> const &regions);
  // void updateBuffer(shared<Buffer> const &dst, u64 dstOffset,
  //                   u64 dataSize, void const *data);
  // void fillBuffer(shared<Buffer> const &dst, u64 dstOffset, u64 size,
  //                 u32 data);
  void clearColorImage(shared<Image> const &image,
                       VkImageLayout const &imageLayout,
                       vec<VkImageSubresourceRange> const &ranges,
                       VkClearColorValue const &clearValue);
  void clearDepthStencilImage(shared<Image> const &image,
                              VkImageLayout const &imageLayout,
                              vec<VkImageSubresourceRange> const &ranges,
                              VkClearDepthStencilValue const &clearValue);
  void clearAttachments(vec<VkClearAttachment> const &attachments,
                        vec<VkClearRect> const &rects);
  void resolveImage(shared<Image> const &src, VkImageLayout const &srcLayout,
                    shared<Image> const &dst, VkImageLayout const &dstLayout,
                    vec<VkImageResolve> const &regions);
  void setEvent(VkEvent event, VkPipelineStageFlags stageMask);
  void resetEvent(VkEvent event, VkPipelineStageFlags stageMask);
  void waitEvents(vec<VkEvent> const &events,
                  VkPipelineStageFlags const &srcStageMask,
                  VkPipelineStageFlags const &dstStageMask,
                  vec<VkMemoryBarrier> const &memoryBarriers,
                  vec<VkBufferMemoryBarrier> const &bufferBarriers,
                  vec<VkImageMemoryBarrier> const &imageBarriers);
  void pipelineBarrier(VkPipelineStageFlags srcStageMask,
                       VkPipelineStageFlags dstStageMask,
                       VkDependencyFlags dependencyFlags,
                       vec<VkMemoryBarrier> const &memoryBarriers,
                       vec<VkBufferMemoryBarrier> const &bufferBarriers,
                       vec<VkImageMemoryBarrier> const &imageBarriers);
  // void beginQuery(VkQueryPool queryPool, u32 query, u32 queryCount); // TODO
  // void endQuery(VkQueryPool queryPool, u32 query, u32 queryCount); // TODO
  // void resetQueryPool(VkQueryPool queryPool, u32 firstQuery, u32 queryCount);
  // // TODO void writeTimestamp(VkPipelineStageFlagBits pipelineStage,
  //                     VkQueryPool queryPool, u32 query); // TODO
  // void copyQueryPoolResults(VkQueryPool queryPool, u32 firstQuery,
  //                           u32 queryCount,
  //                           shared<CommandBuffer> const &dstBuffer,
  //                           u64 dstOffset, u64 stride,
  //                           VkQueryResultFlags flags); // TODO
  // void pushConstants(shared<PipelineLayout> const &layout,
  //                    VkShaderStageFlags stageFlags, u32 offset, u32 size,
  //                    void const *pValues);
  void beginRenderPass(VkRenderPassBeginInfo const &renderPassInfo,
                       VkSubpassContents contents);
  void nextSubpass(VkSubpassContents contents);
  void endRenderPass();
  void executeCommands(vec<shared<CommandBuffer>> const &commandBuffers);
  void begin(CommandBufferUsage usage,
             shared<CommandBuffer> const &inheritFrom = nullptr);
  void end();
  void reset(bool releaseResources = false);

public:
  static shared<CommandBufferEncoder>
  create(shared<CommandBuffer> const &commandBuffer);
};

} // namespace Terreate::Core::Vulkan
