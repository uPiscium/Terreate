#pragma once

#include "common/type.hpp"

#include "core/sdl/window.hpp"

#include "device.hpp"

namespace Terreate::Vulkan {

class Swapchain {
private:
  PROHIBIT_COPY_AND_ASSIGN(Swapchain);

private:
  shared<Device> mDevice = nullptr;
  shared<SDL::Window> mWindow = nullptr;
  VkSwapchainKHR mHandle = VK_NULL_HANDLE;
  vec<VkImage> mImages;
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

  Swapchain(shared<Device> device, shared<SDL::Window> window);

public:
  ~Swapchain();

  u32 getImageCount() const;
  i32 getNextImageIndex(VkSemaphore const &semaphore) const;
  vec<VkImage> const &getImages() const;
  VkFormat getImageFormat() const;
  VkExtent2D getExtent() const;

  void update();

  operator VkSwapchainKHR() const;

public:
  static shared<Swapchain> create(shared<Device> device,
                                  shared<SDL::Window> window);
};

} // namespace Terreate::Vulkan
