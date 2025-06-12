#include "../../include/common/exception.hpp"
#include "../../include/vulkan/command.hpp"

namespace Terreate::Vulkan {
CommandPool::CommandPool(VkObjectRef<Device> device) : mDevice(device) {
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

CommandPool::~CommandPool() {
  for (auto &commandBuffer : mCommandBuffers) {
    commandBuffer.dispose();
  }
  if (mCommandPool != VK_NULL_HANDLE) {
    vkDestroyCommandPool(*mDevice, mCommandPool, nullptr);
  }
}

VkObjectRef<CommandBuffer>
CommandPool::createCommandBuffer(Type::CommandBufferLevel const &level) {
  auto commandBuffer =
      makeVkObject<CommandBuffer>(mDevice, mCommandPool, level);
  mCommandBuffers.push_back(std::move(commandBuffer));
  return mCommandBuffers.back().ref();
}

CommandBuffer::CommandBuffer(VkObjectRef<Device> device, VkCommandPool pool,
                             Type::CommandBufferLevel const &level)
    : mDevice(device), mCommandPool(pool) {
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

CommandBuffer::~CommandBuffer() {
  if (mCommandBuffer != VK_NULL_HANDLE) {
    vkFreeCommandBuffers(*mDevice, mCommandPool, 1, &mCommandBuffer);
  }
}

void CommandBuffer::setRenderPass(VkObjectRef<RenderPass> renderPass,
                                  VkObjectRef<Swapchain> swapchain,
                                  VkObjectRef<Pipeline> pipeline,
                                  VkFramebuffer framebuffer,
                                  Type::vec<float> const &clearColor,
                                  Type::SubpassContent content) {
  if (!mIsRecording) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not recording. Call begin() before setting render "
        "pass.");
  }
  if (mCommandBuffer == VK_NULL_HANDLE) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not allocated. Cannot set render pass.");
  }

  VkClearValue color = {
      {{clearColor[0], clearColor[1], clearColor[2], clearColor[3]}}};
  VkRenderPassBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  beginInfo.renderPass = *renderPass;
  beginInfo.framebuffer = framebuffer;
  beginInfo.renderArea.offset = {0, 0};
  beginInfo.renderArea.extent = swapchain->getProperty().extent;
  beginInfo.clearValueCount = 1;
  beginInfo.pClearValues = &color;

  vkCmdBeginRenderPass(mCommandBuffer, &beginInfo, (VkSubpassContents)content);
  vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
}

void CommandBuffer::setViewport(float x0, float y0, float width, float height,
                                float minDepth, float maxDepth) {
  if (!mIsRecording) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not recording. Call begin() before setting "
        "viewport.");
  }
  if (mCommandBuffer == VK_NULL_HANDLE) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not allocated. Cannot set viewport.");
  }

  VkViewport viewport{};
  viewport.x = x0;
  viewport.y = y0;
  viewport.width = width;
  viewport.height = height;
  viewport.minDepth = minDepth;
  viewport.maxDepth = maxDepth;
  vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);
}

void CommandBuffer::setScissor(Type::i32 offsetX, Type::i32 offsetY,
                               Type::u32 width, Type::u32 height) {
  if (!mIsRecording) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not recording. Call begin() before setting "
        "scissor.");
  }
  if (mCommandBuffer == VK_NULL_HANDLE) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not allocated. Cannot set scissor.");
  }

  VkRect2D scissor{};
  scissor.offset.x = offsetX;
  scissor.offset.y = offsetY;
  scissor.extent.width = width;
  scissor.extent.height = height;
  vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);
}

void CommandBuffer::begin(Util::Bitflag<Type::CommandBufferUsage> flags,
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

  mIsRecording = true;
}

void CommandBuffer::end() {
  vkCmdEndRenderPass(mCommandBuffer);
  if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS) {
    throw Exception::CommandBufferRecordingFailure(
        "Failed to end recording command buffer.");
  }

  mIsRecording = false;
}

void CommandBuffer::drawBuffer(Type::u64 verexCount, Type::u64 instanceCount,
                               Type::u64 firstVertex, Type::u64 firstInstance) {
  if (!mIsRecording) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not recording. Call begin() before drawing.");
  }
  if (mCommandBuffer == VK_NULL_HANDLE) {
    throw Exception::CommandBufferRecordingFailure(
        "Command buffer is not allocated. Cannot draw.");
  }

  vkCmdDraw(mCommandBuffer, verexCount, instanceCount, firstVertex,
            firstInstance);
}

void CommandBuffer::reset() {
  if (mCommandBuffer == VK_NULL_HANDLE) {
    throw Exception::SyncObjectResetFailure(
        "Command buffer is not allocated. Cannot reset.");
  }

  if (vkResetCommandBuffer(mCommandBuffer, 0) != VK_SUCCESS) {
    throw Exception::SyncObjectResetFailure("Failed to reset command buffer.");
  }

  mIsRecording = false;
  mRenderPassInfo = {};
  mContent = VK_SUBPASS_CONTENTS_INLINE;
  mViewport = {};
  mScissor = {};
  mDrawSetting = {};
}

} // namespace Terreate::Vulkan
