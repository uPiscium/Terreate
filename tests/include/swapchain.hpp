#pragma once
#include <api.hpp>
#include <device.hpp>
#include <hubptr.hpp>
#include <type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;
using namespace Terreate::Util::Pointer;

class Swapchain {
private:
  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  vec<VkImage> mImages;
  vec<VkImageView> mImageViews;
  vec<VkFramebuffer> mSFramebuffers;
  VkFormat mImageFormat;
  VkExtent2D mExtent;

private:
  VkSurfaceFormatKHR pickSurfaceFormat(vec<VkSurfaceFormatKHR> const &formats);
  VkPresentModeKHR pickPresentMode(vec<VkPresentModeKHR> const &presentModes);
  VkExtent2D pickExtent(GLFWwindow *window,
                        VkSurfaceCapabilitiesKHR const &capabilities);
  void createSwapchain(GLFWwindow *window, VkSurfaceKHR surface,
                       RefPointer<LogicalDevice> device);
  void createImageViews(RefPointer<LogicalDevice> device);

public:
  Swapchain(GLFWwindow *window, VkSurfaceKHR surface,
            RefPointer<LogicalDevice> device);
};

} // namespace Terreate::Core
