#pragma once
#include "surface.hpp"

#include "../common/type.hpp"

namespace Terreate::Vulkan {
struct QueueFamilyIndices {
  Type::optional<Type::u32> graphicsFamily;
  Type::optional<Type::u32> presentFamily;

  operator Type::set<Type::u32>() const {
    return {graphicsFamily.value_or(0), presentFamily.value_or(0)};
  }
  operator bool() const {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapchainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  Type::vec<VkSurfaceFormatKHR> formats;
  Type::vec<VkPresentModeKHR> presentModes;
};

class IDevice {
public:
  virtual ~IDevice() = default;

  virtual QueueFamilyIndices getQueue() const = 0;
  virtual SwapchainSupportDetails
  getSwapchainSupport(VkObjectRef<ISurface> surface) = 0;

  virtual operator VkDevice() const = 0;
  virtual operator VkPhysicalDevice() const = 0;
};

class Device : public IDevice {
private:
  PROHIBIT_COPY_AND_ASSIGN(Device);

private:
  VkInstance mInstance = VK_NULL_HANDLE;

  QueueFamilyIndices mQueueFamily;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;

private:
  QueueFamilyIndices findQueue(VkPhysicalDevice device,
                               VkObjectRef<ISurface> surface) const;
  int rateDevice(VkPhysicalDevice device, VkObjectRef<ISurface> surface) const;
  bool checkExtSupport(VkPhysicalDevice device);
  SwapchainSupportDetails getSwapchainSupport(VkPhysicalDevice device,
                                              VkObjectRef<ISurface> surface);
  void pickPhysicalDevice(VkObjectRef<ISurface> surface);
  void createLogicalDevice(VkObjectRef<ISurface> surface);

public:
  Device(VkInstance instance, VkObjectRef<ISurface> surface);
  ~Device() override;

  QueueFamilyIndices getQueue() const override { return mQueueFamily; }
  SwapchainSupportDetails
  getSwapchainSupport(VkObjectRef<ISurface> surface) override {
    return this->getSwapchainSupport(mPhysicalDevice, surface);
  }

  operator VkDevice() const override { return mDevice; }
  operator VkPhysicalDevice() const override { return mPhysicalDevice; }
};
} // namespace Terreate::Vulkan
