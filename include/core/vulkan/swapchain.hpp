#pragma once

#include "common/type.hpp"

#include "core/sdl/window.hpp"

#include "device.hpp"
#include "image.hpp"

namespace Terreate::Core::Vulkan {

class Swapchain {
private:
  PROHIBIT_COPY_AND_ASSIGN(Swapchain);

private:
  shared<Device> mDevice = nullptr;
  shared<SDL::Window> mWindow = nullptr;
  VkSwapchainKHR mHandle = VK_NULL_HANDLE;
  vec<VkImage> mImages;
  vec<shared<ImageView>> mImageViews;
  VkFormat mImageFormat;
  VkExtent2D mExtent;

private:
  VkSurfaceFormatKHR
  chooseFormat(vec<VkSurfaceFormatKHR> const &availableFormats);
  VkPresentModeKHR
  choosePresentMode(vec<VkPresentModeKHR> const &availablePresentModes);
  VkExtent2D chooseExtent(VkSurfaceCapabilitiesKHR const &capabilities);
  void createSwapchain();
  void cleanup();

  Swapchain(shared<Device> const &device, shared<SDL::Window> const &window);

public:
  ~Swapchain();

  u32 getImageCount() const;
  i32 getNextImageIndex(VkSemaphore const &semaphore) const;
  // vec<VkImage> const &getImages() const;
  vec<shared<ImageView>> const &getImageViews() const;
  VkFormat getImageFormat() const;
  VkExtent2D getExtent() const;

  void update();

  operator VkSwapchainKHR() const;

public:
  static shared<Swapchain> create(shared<Device> const &device,
                                  shared<SDL::Window> const &window);
};

} // namespace Terreate::Core::Vulkan
