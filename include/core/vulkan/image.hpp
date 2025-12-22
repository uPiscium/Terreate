#pragma once

#include "common/type.hpp"

#include "allocator.hpp"
#include "device.hpp"

namespace Terreate::Core::Vulkan {

class Image {
private:
  PROHIBIT_COPY_AND_ASSIGN(Image);

private:
  shared<Device> mDevice = nullptr;
  shared<Allocator> mAllocator = nullptr;
  ImageAllocation mAllocation{};
  void *mMappedMemory = nullptr;

private:
  Image(shared<Device> const &device, shared<Allocator> const &allocator,
        pair<u32> size, u32 mipLevels, VkSampleCountFlagBits samples,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties);

public:
  ~Image();

  operator VkImage() const;

public:
  static shared<Image> create(shared<Device> const &device,
                              shared<Allocator> const &allocator,
                              pair<u32> size, u32 mipLevels,
                              VkSampleCountFlagBits samples, VkFormat format,
                              VkImageTiling tiling, VkImageUsageFlags usage,
                              VkMemoryPropertyFlags properties);
};

class ImageView {
private:
  PROHIBIT_COPY_AND_ASSIGN(ImageView);

private:
  shared<Device> mDevice = nullptr;
  VkImageView mImageView = VK_NULL_HANDLE;

private:
  ImageView(shared<Device> const &device, VkImage image, VkFormat format,
            VkImageAspectFlags aspectFlags, u32 mipLevels);

public:
  ~ImageView();

  operator VkImageView() const;

public:
  static shared<ImageView> create(shared<Device> const &device, VkImage image,
                                  VkFormat format,
                                  VkImageAspectFlags aspectFlags,
                                  u32 mipLevels);
  static shared<ImageView> create(shared<Device> const &device,
                                  shared<Image> const &image, VkFormat format,
                                  VkImageAspectFlags aspectFlags,
                                  u32 mipLevels);
};

} // namespace Terreate::Core::Vulkan
