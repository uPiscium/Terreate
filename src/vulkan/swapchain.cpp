#include "vulkan/exception.hpp"
#include "vulkan/swapchain.hpp"

namespace Terreate::Vulkan {

VkSurfaceFormatKHR
Swapchain::chooseFormat(vec<VkSurfaceFormatKHR> const &availableFormats) {
  for (auto const &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR Swapchain::choosePresentMode(
    vec<VkPresentModeKHR> const &availablePresentModes) {
  for (auto const &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
Swapchain::chooseExtent(VkSurfaceCapabilitiesKHR const &capabilities) {
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    pair<i32> size = mWindow->getSize();
    VkExtent2D actualExtent = {static_cast<u32>(size.first),
                               static_cast<u32>(size.second)};
    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return actualExtent;
  }
}

void Swapchain::createSwapchain() {
  VkSurfaceCapabilitiesKHR capabilities = mDevice->getSurfaceCapabilities();
  VkSurfaceFormatKHR surfaceFormat =
      this->chooseFormat(mDevice->getSurfaceFormats());
  VkPresentModeKHR presentMode =
      this->choosePresentMode(mDevice->getPresentModes());
  VkExtent2D extent = this->chooseExtent(capabilities);
  u32 imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = *mWindow;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

  createInfo.preTransform = capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(*mDevice, &createInfo, nullptr, &mHandle) !=
      VK_SUCCESS) {
    throw SwapchainException("Failed to create swapchain.");
  }

  vkGetSwapchainImagesKHR(*mDevice, mHandle, &imageCount, nullptr);
  mImages.resize(imageCount);
  vkGetSwapchainImagesKHR(*mDevice, mHandle, &imageCount, mImages.data());

  mImageFormat = surfaceFormat.format;
  mExtent = extent;
}

void Swapchain::cleanup() {
  if (mHandle != VK_NULL_HANDLE) {
    vkDestroySwapchainKHR(*mDevice, mHandle, nullptr);
    mHandle = VK_NULL_HANDLE;
  }
}

Swapchain::Swapchain(shared<Device> device, shared<SDL::Window> window)
    : mDevice(device), mWindow(window) {
  this->createSwapchain();
}

Swapchain::~Swapchain() { this->cleanup(); }

u32 Swapchain::getImageCount() const {
  return static_cast<u32>(mImages.size());
}

i32 Swapchain::getNextImageIndex(VkSemaphore const &semaphore) const {
  u32 imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      *mDevice, mHandle, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    return -1;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw SwapchainException("Failed to acquire swapchain image.");
  }

  return imageIndex;
}

vec<VkImage> const &Swapchain::getImages() const { return mImages; }

VkFormat Swapchain::getImageFormat() const { return mImageFormat; }

VkExtent2D Swapchain::getExtent() const { return mExtent; }

void Swapchain::update() {
  this->cleanup();
  this->createSwapchain();
}

Swapchain::operator VkSwapchainKHR() const { return mHandle; }

shared<Swapchain> Swapchain::create(shared<Device> device,
                                    shared<SDL::Window> window) {
  Swapchain *swapchain = new Swapchain(device, window);
  return shared<Swapchain>(swapchain);
}

} // namespace Terreate::Vulkan
