#include <algorithm>
#include <swapchain.hpp>

namespace Terreate::Core {
VkSurfaceFormatKHR
Swapchain::pickSurfaceFormat(vec<VkSurfaceFormatKHR> const &formats) {
  for (auto const &format : formats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  return formats[0];
}

VkPresentModeKHR
Swapchain::pickPresentMode(vec<VkPresentModeKHR> const &presentModes) {
  for (auto const &presentMode : presentModes) {
    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return presentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::pickExtent(GLFWwindow *window,
                                 VkSurfaceCapabilitiesKHR const &capabilities) {
  if (capabilities.currentExtent.width != numlimit<u32>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    VkExtent2D extent = {static_cast<uint32_t>(width),
                         static_cast<uint32_t>(height)};
    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width,
                              capabilities.maxImageExtent.width);
    extent.height =
        std::clamp(extent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return extent;
  }
}

void Swapchain::createSwapchain(GLFWwindow *window, VkSurfaceKHR surface,
                                RefPointer<LogicalDevice> device) {
  API::SwapchainSupport details = API::getSwapchainSupport(*device, surface);
  VkSurfaceFormatKHR surfaceFormat = this->pickSurfaceFormat(details.formats);
  VkPresentModeKHR presentMode = this->pickPresentMode(details.presentModes);
  VkExtent2D extent = this->pickExtent(window, details.capabilities);

  uint32_t const &minImgCount = details.capabilities.minImageCount;
  uint32_t const &maxImgCount = details.capabilities.maxImageCount;
  uint32_t imgCount = minImgCount + 1;
  imgCount =
      ((maxImgCount > 0) && (imgCount > maxImgCount)) ? maxImgCount : imgCount;

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imgCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  set<u32> indices = (set<u32>)device->getQueueFamilies(surface);
  vec<u32> queueFamily(indices.begin(), indices.end());

  if (queueFamily.size() > 1) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount =
        static_cast<uint32_t>(queueFamily.size());
    createInfo.pQueueFamilyIndices = queueFamily.data();
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

  if (vkCreateSwapchainKHR(*device, &createInfo, nullptr, &mSwapchain) !=
      VK_SUCCESS) {
    throw Exception::SwapchainCreationFailure();
  }

  mImages = API::getSwapchainImages(*device, mSwapchain);
  mImageFormat = surfaceFormat.format;
  mExtent = extent;
}

void Swapchain::createImageViews(RefPointer<LogicalDevice> device) {
  mImageViews.resize(mImages.size());
  for (size_t i = 0; i < mImages.size(); ++i) {
    mImageViews[i] = API::createImageView(*device, mImages[i], mImageFormat);
  }
}

Swapchain::Swapchain(GLFWwindow *window, VkSurfaceKHR surface,
                     RefPointer<LogicalDevice> device) {
  this->createSwapchain(window, surface, device);
  this->createImageViews(device);
}

} // namespace Terreate::Core
