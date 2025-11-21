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
  CommandPool *commandPool = new CommandPool(device, transient, protect);
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

void CommandBuffer::begin(
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

  if (vkBeginCommandBuffer(mHandle, &beginInfo) != VK_SUCCESS) {
    throw CommandException("Failed to begin recording command buffer.");
  }
}

void CommandBuffer::end() {
  if (vkEndCommandBuffer(mHandle) != VK_SUCCESS) {
    throw CommandException("Failed to end recording command buffer.");
  }
}

void CommandBuffer::reset(bool releaseResources) {
  VkCommandBufferResetFlags flags = 0;
  if (releaseResources) {
    flags |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
  }
  if (vkResetCommandBuffer(mHandle, flags) != VK_SUCCESS) {
    throw CommandException("Failed to reset command buffer.");
  }
}

CommandBuffer::operator VkCommandBuffer() const { return mHandle; }

shared<CommandBuffer>
CommandBuffer::create(shared<Device> const &device,
                      shared<CommandPool> const &commandPool,
                      CommandBufferLevel level) {
  CommandBuffer *commandBuffer = new CommandBuffer(device, commandPool, level);
  return shared<CommandBuffer>(commandBuffer);
}

} // namespace Terreate::Vulkan
