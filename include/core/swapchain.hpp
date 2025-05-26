#pragma once
#include "device.hpp"

#include "../decl/type.hpp"
#include "../util/resourceptr.hpp"

namespace Terreate::Core {
struct SwapchainProperty {
  VkFormat imageFormat;
  VkExtent2D extent;
};

class Swapchain {
private:
  PROHIBIT_COPY_AND_ASSIGN(Swapchain);

private:
  VkSurfaceFormatKHR
  pickSurfaceFormat(Type::vec<VkSurfaceFormatKHR> const &formats);
  VkPresentModeKHR
  pickPresentMode(Type::vec<VkPresentModeKHR> const &presentModes);
  VkExtent2D pickExtent(Type::pair<Type::i32> framebufferSize,
                        VkSurfaceCapabilitiesKHR const &capabilities);
  void createSwapchain(Type::pair<Type::i32> framebufferSize,
                       VkSurfaceKHR surface);
  void createImageViews();

private:
  Util::ResourcePointer<Device> mDevice;

  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  Type::vec<VkImage> mSwapchainImages;
  Type::vec<VkImageView> mSwapchainImageViews;
  SwapchainProperty mSwapchainProperty;

public:
  Swapchain(Util::ResourcePointer<Device> device,
            Type::pair<Type::i32> framebufferSize, VkSurfaceKHR surface);
  ~Swapchain() { this->destroy(); }

  Util::ResourcePointer<Device> getDevice() const { return mDevice; }
  SwapchainProperty const &getProperty() const { return mSwapchainProperty; }
  Type::vec<VkImageView> const &getImageViews() const {
    return mSwapchainImageViews;
  }

  void destroy();
};
} // namespace Terreate::Core
