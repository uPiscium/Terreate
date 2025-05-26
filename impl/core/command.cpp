#include "../../include/core/command.hpp"
#include "../../include/decl/exception.hpp"

namespace Terreate::Core {
CommandPool::CommandPool(Util::ResourcePointer<Pipeline> pipeline)
    : mPipeline(pipeline), mDevice(pipeline->getDevice()) {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = mDevice->getQueue().graphicsFamily.value();

  if (vkCreateCommandPool(*mDevice, &poolInfo, nullptr, &mCommandPool) !=
      VK_SUCCESS) {
    throw Exception::CommandPoolCreationFailure(
        "Failed to create command pool.");
  }
}

Util::ResourcePointer<CommandBuffer>
CommandPool::createCommandBuffer(Type::CommandBufferLevel const &level) {
  auto commandBuffer =
      Util::createResource<CommandBuffer>(mPipeline, mCommandPool, level);
  mCommandBuffers.push_back(std::move(commandBuffer));
  return mCommandBuffers.back().get();
}

void CommandPool::dispose() {
  for (auto &commandBuffer : mCommandBuffers) {
    commandBuffer.dispose();
  }
  if (mCommandPool != VK_NULL_HANDLE) {
    vkDestroyCommandPool(*mDevice, mCommandPool, nullptr);
  }
}

void CommandBuffer::begin(Bitflag<Type::CommandBufferUsage> flags,
                          VkCommandBufferInheritanceInfo *inherit) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags =
      (VkCommandBufferUsageFlags)((Type::CommandBufferUsage)flags);
  if (inherit) {
    beginInfo.pInheritanceInfo = inherit;
  } else {
    beginInfo.pInheritanceInfo = nullptr;
  }

  if (vkBeginCommandBuffer(mCommandBuffer, &beginInfo) != VK_SUCCESS) {
    throw Exception::CommandBufferRecordingFailure(
        "Failed to begin recording command buffer.");
  }
}

void CommandBuffer::end() {
  if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS) {
    throw Exception::CommandBufferRecordingFailure(
        "Failed to end recording command buffer.");
  }
}

CommandBuffer::CommandBuffer(Util::ResourcePointer<Pipeline> pipeline,
                             VkCommandPool pool,
                             Type::CommandBufferLevel const &level)
    : mPipeline(pipeline), mDevice(pipeline->getDevice()), mCommandPool(pool) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = mCommandPool;
  allocInfo.level = (VkCommandBufferLevel)level;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(*mDevice, &allocInfo, &mCommandBuffer) !=
      VK_SUCCESS) {
    throw Exception::CommandBufferCreationFailure(
        "Failed to allocate command buffer.");
  }
}

void CommandBuffer::setRenderPass(VkFramebuffer framebuffer,
                                  Type::vec<float> const &clearColor,
                                  Type::SubpassContent content) {
  if (!mIsRecording) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not ready to record. Call begin() function before "
        "calling.");
  }

  mRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  mRenderPassInfo.renderPass = *mPipeline;
  mRenderPassInfo.framebuffer = framebuffer;
  mRenderPassInfo.renderArea.offset = {0, 0};
  mRenderPassInfo.renderArea.extent =
      mPipeline->getSwapchain()->getProperty().extent;
  VkClearValue color = {
      {{clearColor[0], clearColor[1], clearColor[2], clearColor[3]}}};
  mRenderPassInfo.clearValueCount = 1;
  mRenderPassInfo.pClearValues = &color;

  mContent = (VkSubpassContents)content;
}

void CommandBuffer::setViewport(float x0, float y0, float width, float height,
                                float minDepth, float maxDepth) {
  if (!mIsRecording) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not ready to record. Call begin() function before "
        "calling.");
  }

  mViewport.x = x0;
  mViewport.y = y0;
  mViewport.width = width;
  mViewport.height = height;
  mViewport.minDepth = minDepth;
  mViewport.maxDepth = maxDepth;
}

void CommandBuffer::setScissor(Type::i32 offsetX, Type::i32 offsetY,
                               Type::u32 width, Type::u32 height) {
  mScissor.offset = {offsetX, offsetY};
  mScissor.extent = {width, height};
}

void CommandBuffer::drawBuffer(Type::u64 verexCount, Type::u64 instanceCount,
                               Type::u64 firstVertex, Type::u64 firstInstance) {
  mDrawSetting.verexCount = verexCount;
  mDrawSetting.instanceCount = instanceCount;
  mDrawSetting.firstVertex = firstVertex;
  mDrawSetting.firstInstance = firstInstance;
}

void CommandBuffer::compile(Bitflag<Type::CommandBufferUsage> flags,
                            VkCommandBufferInheritanceInfo *inherit) {
  this->begin(flags, inherit);
  vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    *mPipeline);
  vkCmdBeginRenderPass(mCommandBuffer, &mRenderPassInfo, mContent);
  vkCmdSetViewport(mCommandBuffer, 0, 1, &mViewport);
  vkCmdSetScissor(mCommandBuffer, 0, 1, &mScissor);
  vkCmdDraw(mCommandBuffer, mDrawSetting.verexCount, mDrawSetting.instanceCount,
            mDrawSetting.firstVertex, mDrawSetting.instanceCount);
  vkCmdEndRenderPass(mCommandBuffer);
  this->end();
}

void CommandBuffer::dispose() {
  if (mCommandBuffer != VK_NULL_HANDLE) {
    vkFreeCommandBuffers(*mDevice, mCommandPool, 1, &mCommandBuffer);
  }
}
} // namespace Terreate::Core
