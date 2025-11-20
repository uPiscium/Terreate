#include "core/vulkan/allocator.hpp"
#include "core/vulkan/exception.hpp"

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

namespace Terreate::Vulkan {

Allocator::Allocator(shared<Instance> instance, shared<Device> device)
    : mInstance(instance), mDevice(device) {
  VmaAllocatorCreateInfo allocatorInfo{};
  allocatorInfo.device = *mDevice;
  allocatorInfo.physicalDevice = *mDevice->getPhysicalDevice();
  allocatorInfo.instance = *mInstance;

  if (vmaCreateAllocator(&allocatorInfo, &mHandle) != VK_SUCCESS) {
    throw AllocatorException("failed to create VMA allocator!");
  }
}

Allocator::~Allocator() {
  vmaDestroyAllocator(mHandle);
  mHandle = VK_NULL_HANDLE;
}

BufferAllocation Allocator::create(VkDeviceSize size, VkBufferUsageFlags usage,
                                   VkMemoryPropertyFlags properties) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo allocCreateInfo{};
  allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

  BufferAllocation allocation{};
  vmaCreateBuffer(mHandle, &bufferInfo, &allocCreateInfo, &allocation.buffer,
                  &allocation.allocation, nullptr);

  return allocation;
}

ImageAllocation Allocator::create(pair<u32> size, u32 mipLevels,
                                  VkSampleCountFlagBits samples,
                                  VkFormat format, VkImageTiling tiling,
                                  VkImageUsageFlags usage,
                                  VkMemoryPropertyFlags properties) {
  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = size.first;
  imageInfo.extent.height = size.second;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = mipLevels;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.samples = samples;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.flags = 0;

  VmaAllocationCreateInfo allocCreateInfo{};
  allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

  ImageAllocation allocation{};
  vmaCreateImage(mHandle, &imageInfo, &allocCreateInfo, &allocation.image,
                 &allocation.allocation, nullptr);

  return allocation;
}

void Allocator::destroy(BufferAllocation &allocation) {
  vmaDestroyBuffer(mHandle, allocation.buffer, allocation.allocation);
  allocation.buffer = VK_NULL_HANDLE;
  allocation.allocation = VK_NULL_HANDLE;
}

void Allocator::destroy(ImageAllocation &allocation) {
  vmaDestroyImage(mHandle, allocation.image, allocation.allocation);
  allocation.image = VK_NULL_HANDLE;
  allocation.allocation = VK_NULL_HANDLE;
}

} // namespace Terreate::Vulkan
