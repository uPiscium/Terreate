#include "core/vulkan/exception.hpp"
#include "core/vulkan/image.hpp"

namespace Terreate::Vulkan {

Image::Image(shared<Device> const &device, shared<Allocator> const &allocator,
             pair<u32> size, u32 mipLevels, VkSampleCountFlagBits samples,
             VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
             VkMemoryPropertyFlags properties)
    : mDevice(device), mAllocator(allocator) {
  mAllocation = allocator->create(size, mipLevels, samples, format, tiling,
                                  usage, properties);
}

Image::~Image() { mAllocator->destroy(mAllocation); }

Image::operator VkImage() const { return mAllocation.image; }

shared<Image> Image::create(shared<Device> const &device,
                            shared<Allocator> const &allocator, pair<u32> size,
                            u32 mipLevels, VkSampleCountFlagBits samples,
                            VkFormat format, VkImageTiling tiling,
                            VkImageUsageFlags usage,
                            VkMemoryPropertyFlags properties) {
  Image *image = new Image(device, allocator, size, mipLevels, samples, format,
                           tiling, usage, properties);
  return shared<Image>(image);
}

ImageView::ImageView(shared<Device> const &device, VkImage image,
                     VkFormat format, VkImageAspectFlags aspectFlags,
                     u32 mipLevels)
    : mDevice(device) {
  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = mipLevels;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  if (vkCreateImageView(*mDevice, &viewInfo, nullptr, &mImageView) !=
      VK_SUCCESS) {
    throw ImageException("Failed to create image view.");
  }
}

ImageView::~ImageView() {
  if (mImageView != VK_NULL_HANDLE) {
    vkDestroyImageView(*mDevice, mImageView, nullptr);
    mImageView = VK_NULL_HANDLE;
  }
}

ImageView::operator VkImageView() const { return mImageView; }

shared<ImageView> ImageView::create(shared<Device> const &device, VkImage image,
                                    VkFormat format,
                                    VkImageAspectFlags aspectFlags,
                                    u32 mipLevels) {
  ImageView *imageView =
      new ImageView(device, image, format, aspectFlags, mipLevels);
  return shared<ImageView>(imageView);
}

shared<ImageView> ImageView::create(shared<Device> const &device,
                                    shared<Image> const &image, VkFormat format,
                                    VkImageAspectFlags aspectFlags,
                                    u32 mipLevels) {
  return ImageView::create(device, *image, format, aspectFlags, mipLevels);
}

} // namespace Terreate::Vulkan
