#include "common/type.hpp"
#include "core/vulkan/command.hpp"
#include "core/vulkan/exception.hpp"

namespace Terreate::Vulkan {

CommandPool::CommandPool(shared<Device> const &device, bool transient,
                         bool protect)
    : mDevice(device) {
  umap<Vulkan::QueueType, u32> queueFamilyIndices =
      mDevice->getQueueFamilyIndices();
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  if (transient) {
    poolInfo.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
  }
  if (protect) {
    poolInfo.flags |= VK_COMMAND_POOL_CREATE_PROTECTED_BIT;
  }
  poolInfo.queueFamilyIndex = queueFamilyIndices[Vulkan::QueueType::GRAPHICS];

  if (vkCreateCommandPool(*mDevice, &poolInfo, nullptr, &mHandle) !=
      VK_SUCCESS) {
    throw CommandException("Failed to create command pool.");
  }
}

CommandPool::~CommandPool() {
  if (mHandle != VK_NULL_HANDLE) {
    vkDestroyCommandPool(*mDevice, mHandle, nullptr);
    mHandle = VK_NULL_HANDLE;
  }
}

CommandPool::operator VkCommandPool() const { return mHandle; }

shared<CommandPool> CommandPool::create(shared<Device> const &device,
                                        bool transient, bool protect) {
  auto *commandPool = new CommandPool(device, transient, protect);
  return shared<CommandPool>(commandPool);
}

CommandBuffer::CommandBuffer(shared<Device> const &device,
                             shared<CommandPool> const &commandPool,
                             CommandBufferLevel level)
    : mDevice(device), mCommandPool(commandPool) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = (VkCommandBufferLevel)level;
  allocInfo.commandPool = *mCommandPool;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(*mDevice, &allocInfo, &mHandle) != VK_SUCCESS) {
    throw CommandException("Failed to allocate command buffer.");
  }
}

CommandBuffer::~CommandBuffer() {
  if (mHandle != VK_NULL_HANDLE) {
    vkFreeCommandBuffers(*mDevice, *mCommandPool, 1, &mHandle);
    mHandle = VK_NULL_HANDLE;
  }
}

CommandBuffer::operator VkCommandBuffer() const { return mHandle; }

shared<CommandBuffer>
CommandBuffer::create(shared<Device> const &device,
                      shared<CommandPool> const &commandPool,
                      CommandBufferLevel level) {
  auto *commandBuffer = new CommandBuffer(device, commandPool, level);
  return shared<CommandBuffer>(commandBuffer);
}

CommandBufferEncoder::CommandBufferEncoder(
    shared<CommandBuffer> const &commandBuffer)
    : mCommandBuffer(commandBuffer) {}

CommandBufferEncoder::~CommandBufferEncoder() {}

shared<CommandBuffer> const &CommandBufferEncoder::getCommandBuffer() const {
  return mCommandBuffer;
}

void CommandBufferEncoder::setViewport(Viewport const &viewport) {
  vkCmdSetViewport(*mCommandBuffer, 0, 1, &viewport);
}

void CommandBufferEncoder::setViewports(vec<Viewport> const &viewports,
                                        u32 firstViewport) {
  vkCmdSetViewport(*mCommandBuffer, firstViewport,
                   static_cast<u32>(viewports.size()), viewports.data());
}

void CommandBufferEncoder::setScissor(Rect2D const &scissor) {
  vkCmdSetScissor(*mCommandBuffer, 0, 1, &scissor);
}

void CommandBufferEncoder::setScissors(vec<Rect2D> const &scissors,
                                       u32 firstScissor) {
  vkCmdSetScissor(*mCommandBuffer, firstScissor,
                  static_cast<u32>(scissors.size()), scissors.data());
}

void CommandBufferEncoder::setLineWidth(float lineWidth) {
  vkCmdSetLineWidth(*mCommandBuffer, lineWidth);
}

void CommandBufferEncoder::setDepthBias(float constant, float clamp,
                                        float slope) {
  vkCmdSetDepthBias(*mCommandBuffer, constant, clamp, slope);
}

void CommandBufferEncoder::setBlendConstants(vec4 const &blendConstants) {
  vec<float> blendConstantsVec = {blendConstants[0], blendConstants[1],
                                  blendConstants[2], blendConstants[3]};
  vkCmdSetBlendConstants(*mCommandBuffer, blendConstantsVec.data());
}

void CommandBufferEncoder::setDepthBounds(float minDepth, float maxDepth) {
  vkCmdSetDepthBounds(*mCommandBuffer, minDepth, maxDepth);
}

void CommandBufferEncoder::setStencilCompareMask(FaceMask faceMask,
                                                 u32 compareMask) {
  vkCmdSetStencilCompareMask(*mCommandBuffer, (VkStencilFaceFlags)faceMask,
                             compareMask);
}

void CommandBufferEncoder::setStencilWriteMask(FaceMask faceMask,
                                               u32 writeMask) {
  vkCmdSetStencilWriteMask(*mCommandBuffer, (VkStencilFaceFlags)faceMask,
                           writeMask);
}

void CommandBufferEncoder::setStencilReference(FaceMask faceMask,
                                               u32 reference) {
  vkCmdSetStencilReference(*mCommandBuffer, (VkStencilFaceFlags)faceMask,
                           reference);
}

bool CommandBufferEncoder::isEncoding() const { return mIsEncoding; }

void CommandBufferEncoder::draw(u32 vertexCount, u32 instanceCount,
                                u32 firstVertex, u32 firstInstance) {
  vkCmdDraw(*mCommandBuffer, vertexCount, instanceCount, firstVertex,
            firstInstance);
}

void CommandBufferEncoder::drawIndexed(u32 indexCount, u32 instanceCount,
                                       u32 firstIndex, i32 vertexOffset,
                                       u32 firstInstance) {
  vkCmdDrawIndexed(*mCommandBuffer, indexCount, instanceCount, firstIndex,
                   vertexOffset, firstInstance);
}

void CommandBufferEncoder::dispatch(u32 groupCountX, u32 groupCountY,
                                    u32 groupCountZ) {
  vkCmdDispatch(*mCommandBuffer, groupCountX, groupCountY, groupCountZ);
}

void CommandBufferEncoder::copyImage(shared<Image> const &src,
                                     VkImageLayout const &srcLayout,
                                     shared<Image> const &dst,
                                     VkImageLayout const &dstLayout,
                                     vec<VkImageCopy> const &regions) {
  vkCmdCopyImage(*mCommandBuffer, *src, srcLayout, *dst, dstLayout,
                 static_cast<u32>(regions.size()), regions.data());
}

void CommandBufferEncoder::blitImage(shared<Image> const &src,
                                     VkImageLayout const &srcLayout,
                                     shared<Image> const &dst,
                                     VkImageLayout const &dstLayout,
                                     vec<VkImageBlit> const &regions,
                                     VkFilter filter) {
  vkCmdBlitImage(*mCommandBuffer, *src, srcLayout, *dst, dstLayout,
                 static_cast<u32>(regions.size()), regions.data(), filter);
}

void CommandBufferEncoder::clearColorImage(
    shared<Image> const &image, VkImageLayout const &imageLayout,
    vec<VkImageSubresourceRange> const &ranges,
    VkClearColorValue const &clearValue) {
  vkCmdClearColorImage(*mCommandBuffer, *image, imageLayout, &clearValue,
                       ranges.size(), ranges.data());
}

void CommandBufferEncoder::clearDepthStencilImage(
    shared<Image> const &image, VkImageLayout const &imageLayout,
    vec<VkImageSubresourceRange> const &ranges,
    VkClearDepthStencilValue const &clearValue) {
  vkCmdClearDepthStencilImage(*mCommandBuffer, *image, imageLayout, &clearValue,
                              ranges.size(), ranges.data());
}

void CommandBufferEncoder::clearAttachments(
    vec<VkClearAttachment> const &attachments,
    vec<VkClearRect> const &clearRects) {
  vkCmdClearAttachments(*mCommandBuffer, static_cast<u32>(attachments.size()),
                        attachments.data(), static_cast<u32>(clearRects.size()),
                        clearRects.data());
}

void CommandBufferEncoder::resolveImage(shared<Image> const &src,
                                        VkImageLayout const &srcLayout,
                                        shared<Image> const &dst,
                                        VkImageLayout const &dstLayout,
                                        vec<VkImageResolve> const &regions) {
  vkCmdResolveImage(*mCommandBuffer, *src, srcLayout, *dst, dstLayout,
                    static_cast<u32>(regions.size()), regions.data());
}

void CommandBufferEncoder::setEvent(VkEvent event,
                                    VkPipelineStageFlags stageMask) {
  vkCmdSetEvent(*mCommandBuffer, event, stageMask);
}

void CommandBufferEncoder::resetEvent(VkEvent event,
                                      VkPipelineStageFlags stageMask) {
  vkCmdResetEvent(*mCommandBuffer, event, stageMask);
}

void CommandBufferEncoder::waitEvents(
    vec<VkEvent> const &events, VkPipelineStageFlags const &srcStageMask,
    VkPipelineStageFlags const &dstStageMask,
    vec<VkMemoryBarrier> const &memoryBarriers,
    vec<VkBufferMemoryBarrier> const &bufferBarriers,
    vec<VkImageMemoryBarrier> const &imageBarriers) {
  vkCmdWaitEvents(
      *mCommandBuffer, static_cast<u32>(events.size()), events.data(),
      srcStageMask, dstStageMask, static_cast<u32>(memoryBarriers.size()),
      memoryBarriers.data(), static_cast<u32>(bufferBarriers.size()),
      bufferBarriers.data(), static_cast<u32>(imageBarriers.size()),
      imageBarriers.data());
}

void CommandBufferEncoder::pipelineBarrier(
    VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
    VkDependencyFlags dependencyFlags,
    vec<VkMemoryBarrier> const &memoryBarriers,
    vec<VkBufferMemoryBarrier> const &bufferBarriers,
    vec<VkImageMemoryBarrier> const &imageBarriers) {
  vkCmdPipelineBarrier(
      *mCommandBuffer, srcStageMask, dstStageMask, dependencyFlags,
      static_cast<u32>(memoryBarriers.size()), memoryBarriers.data(),
      static_cast<u32>(bufferBarriers.size()), bufferBarriers.data(),
      static_cast<u32>(imageBarriers.size()), imageBarriers.data());
}

void CommandBufferEncoder::beginRenderPass(
    VkRenderPassBeginInfo const &renderPassInfo, VkSubpassContents contents) {
  vkCmdBeginRenderPass(*mCommandBuffer, &renderPassInfo, contents);
}

void CommandBufferEncoder::nextSubpass(VkSubpassContents contents) {
  vkCmdNextSubpass(*mCommandBuffer, contents);
}

void CommandBufferEncoder::endRenderPass() {
  vkCmdEndRenderPass(*mCommandBuffer);
}

void CommandBufferEncoder::executeCommands(
    vec<shared<CommandBuffer>> const &commandBuffers) {
  vec<VkCommandBuffer> vkCommandBuffers;
  for (auto const &cmdBuf : commandBuffers) {
    vkCommandBuffers.push_back(*cmdBuf);
  }
  vkCmdExecuteCommands(*mCommandBuffer,
                       static_cast<u32>(vkCommandBuffers.size()),
                       vkCommandBuffers.data());
}

void CommandBufferEncoder::begin(
    CommandBufferUsage usage,
    shared<CommandBuffer> const &inheritFrom /*= nullptr*/) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = (VkCommandBufferUsageFlags)usage;

  VkCommandBufferInheritanceInfo inheritanceInfo{};
  if (inheritFrom != nullptr) {
    inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    inheritanceInfo.renderPass = VK_NULL_HANDLE;
    inheritanceInfo.subpass = 0;
    inheritanceInfo.framebuffer = VK_NULL_HANDLE;
    inheritanceInfo.occlusionQueryEnable = VK_FALSE;
    inheritanceInfo.queryFlags = 0;
    inheritanceInfo.pipelineStatistics = 0;
    beginInfo.pInheritanceInfo = &inheritanceInfo;
  } else {
    beginInfo.pInheritanceInfo = nullptr;
  }

  if (vkBeginCommandBuffer(*mCommandBuffer, &beginInfo) != VK_SUCCESS) {
    throw CommandException("Failed to begin recording command buffer.");
  }

  mIsEncoding = true;
}

void CommandBufferEncoder::end() {
  if (vkEndCommandBuffer(*mCommandBuffer) != VK_SUCCESS) {
    throw CommandException("Failed to end recording command buffer.");
  }

  mIsEncoding = false;
}

void CommandBufferEncoder::reset(bool releaseResources) {
  VkCommandBufferResetFlags flags = 0;
  if (releaseResources) {
    flags |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
  }
  if (vkResetCommandBuffer(*mCommandBuffer, flags) != VK_SUCCESS) {
    throw CommandException("Failed to reset command buffer.");
  }
}

shared<CommandBufferEncoder>
CommandBufferEncoder::create(shared<CommandBuffer> const &commandBuffer) {
  auto *encoder = new CommandBufferEncoder(commandBuffer);
  return shared<CommandBufferEncoder>(encoder);
}

} // namespace Terreate::Vulkan
