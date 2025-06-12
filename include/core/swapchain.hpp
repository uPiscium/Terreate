#pragma once
#include "device.hpp"
#include "sync.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
struct SwapchainProperty {
  VkFormat imageFormat;
  VkExtent2D extent;
};

class ISwapchain {
public:
  virtual ~ISwapchain() = default;

  virtual SwapchainProperty const &getProperty() const = 0;
  virtual Type::vec<VkImageView> const &getImageViews() const = 0;
  virtual Type::u32
  getNextImageIndex(VkObjectRef<ISemaphore> semaphore) const = 0;

  virtual operator VkSwapchainKHR() const = 0;
};

class Swapchain : public ISwapchain {
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
                       VkObjectRef<ISurface> surface);
  void createImageViews();

private:
  VkObjectRef<Device> mDevice;

  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  Type::vec<VkImage> mSwapchainImages;
  Type::vec<VkImageView> mSwapchainImageViews;
  SwapchainProperty mSwapchainProperty;

public:
  Swapchain(VkObjectRef<Device> device, Type::pair<Type::i32> framebufferSize,
            VkObjectRef<ISurface> surface);
  ~Swapchain() override;

  SwapchainProperty const &getProperty() const override {
    return mSwapchainProperty;
  }
  Type::vec<VkImageView> const &getImageViews() const override {
    return mSwapchainImageViews;
  }
  Type::u32 getNextImageIndex(VkObjectRef<ISemaphore> semaphore) const override;

  operator VkSwapchainKHR() const override { return mSwapchain; }
};
} // namespace Terreate::Core
