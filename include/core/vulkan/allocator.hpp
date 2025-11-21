#pragma once

#include <vma/vk_mem_alloc.h>

#include "common/type.hpp"

#include "device.hpp"
#include "instance.hpp"

namespace Terreate::Vulkan {

struct BufferAllocation {
  VkBuffer buffer = VK_NULL_HANDLE;
  VmaAllocation allocation = VK_NULL_HANDLE;
};

struct ImageAllocation {
  VkImage image = VK_NULL_HANDLE;
  VmaAllocation allocation = VK_NULL_HANDLE;
};

class Allocator {
private:
  PROHIBIT_COPY_AND_ASSIGN(Allocator);

private:
  shared<Instance> mInstance = nullptr;
  shared<Device> mDevice = nullptr;
  VmaAllocator mHandle = VK_NULL_HANDLE;

private:
  Allocator(shared<Instance> const &instance, shared<Device> const &device);

public:
  ~Allocator();

  BufferAllocation create(VkDeviceSize size, VkBufferUsageFlags usage,
                          VkMemoryPropertyFlags properties);
  ImageAllocation create(pair<u32> size, u32 mipLevels,
                         VkSampleCountFlagBits samples, VkFormat format,
                         VkImageTiling tiling, VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties);
  void destroy(BufferAllocation &allocation);
  void destroy(ImageAllocation &allocation);

  VmaAllocator operator*() const;

public:
  static shared<Allocator> create(shared<Instance> const &instance,
                                  shared<Device> const &device);
};

} // namespace Terreate::Vulkan
