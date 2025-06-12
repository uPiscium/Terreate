#include "../../include/vulkan/swapchain.hpp"

#include <algorithm>
#include <limits>

namespace Terreate::Vulkan {
VkSurfaceFormatKHR Swapchain::pickSurfaceFormat(
    Type::vec<VkSurfaceFormatKHR> const &availableFormats) {
  for (auto const &format : availableFormats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR Swapchain::pickPresentMode(
    Type::vec<VkPresentModeKHR> const &availablePresentModes) {
  for (auto const &presentMode : availablePresentModes) {
    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return presentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::pickExtent(Type::pair<Type::i32> framebufferSize,
                                 VkSurfaceCapabilitiesKHR const &capabilities) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<Type::u32>::max()) {
    return capabilities.currentExtent;
  } else {
    VkExtent2D extent = {(Type::u32)framebufferSize.first,
                         (Type::u32)framebufferSize.second};
    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width,
                              capabilities.maxImageExtent.width);
    extent.height =
        std::clamp(extent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return extent;
  }
}

void Swapchain::createSwapchain(Type::pair<Type::i32> framebufferSize,
                                VkObjectRef<Surface> surface) {
  SwapchainSupportDetails details = mDevice->getSwapchainSupport(surface);
  VkSurfaceFormatKHR surfaceFormat = this->pickSurfaceFormat(details.formats);
  VkPresentModeKHR presentMode = this->pickPresentMode(details.presentModes);
  VkExtent2D extent = this->pickExtent(framebufferSize, details.capabilities);

  Type::u32 const &minImgCount = details.capabilities.minImageCount;
  Type::u32 const &maxImgCount = details.capabilities.maxImageCount;
  Type::u32 imgCount = minImgCount + 1;
  imgCount =
      ((maxImgCount > 0) && (imgCount > maxImgCount)) ? maxImgCount : imgCount;

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = *surface;
  createInfo.minImageCount = imgCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = mDevice->getQueue();
  Type::u32 queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                    indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;     // optional
    createInfo.pQueueFamilyIndices = nullptr; // optional
  }

  createInfo.preTransform = details.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(*mDevice, &createInfo, nullptr, &mSwapchain) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create swapchain.");
  }

  Type::u32 swapchainImgCount;
  vkGetSwapchainImagesKHR(*mDevice, mSwapchain, &swapchainImgCount, nullptr);
  mSwapchainImages.resize(swapchainImgCount);
  vkGetSwapchainImagesKHR(*mDevice, mSwapchain, &swapchainImgCount,
                          mSwapchainImages.data());

  mSwapchainProperty.imageFormat = surfaceFormat.format;
  mSwapchainProperty.extent = extent;
}

void Swapchain::createImageViews() {
  mSwapchainImageViews.resize(mSwapchainImages.size());
  for (Type::u64 i = 0; i < mSwapchainImages.size(); ++i) {
    VkImageViewCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = mSwapchainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = mSwapchainProperty.imageFormat;

    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    createInfo.pNext = nullptr;

    if (vkCreateImageView(*mDevice, &createInfo, nullptr,
                          &mSwapchainImageViews[i]) != VK_SUCCESS) {
      throw Exception::ImageViewCreationFailure(
          "Failed to create image view for swapchain image.");
    }
  }
}

Swapchain::Swapchain(VkObjectRef<Device> device,
                     Type::pair<Type::i32> framebufferSize,
                     VkObjectRef<Surface> surface)
    : mDevice(device) {
  this->createSwapchain(framebufferSize, surface);
  this->createImageViews();
}

Swapchain::~Swapchain() {
  for (auto imageView : mSwapchainImageViews) {
    vkDestroyImageView(*mDevice, imageView, nullptr);
  }
  vkDestroySwapchainKHR(*mDevice, mSwapchain, nullptr);
  mSwapchain = VK_NULL_HANDLE;
}

Type::u32 Swapchain::getNextImageIndex(VkObjectRef<Semaphore> semaphore) const {
  VkSemaphore semaphoreHandle = VK_NULL_HANDLE;
  if (semaphore) {
    semaphoreHandle = *semaphore;
  }

  Type::u32 imageIndex;
  VkResult result =
      vkAcquireNextImageKHR(*mDevice, mSwapchain, UINT64_MAX, semaphoreHandle,
                            VK_NULL_HANDLE, &imageIndex);
  return imageIndex;
}
} // namespace Terreate::Vulkan
