#pragma once
#include "../decl/type.hpp"

namespace Terreate::Core {
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

class Device {
private:
  PROHIBIT_COPY_AND_ASSIGN(Device);

private:
  VkInstance mInstance = VK_NULL_HANDLE;

  QueueFamilyIndices mQueueFamily;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;

private:
  QueueFamilyIndices findQueue(VkPhysicalDevice device,
                               VkSurfaceKHR surface) const;
  int rateDevice(VkPhysicalDevice device, VkSurfaceKHR surface) const;
  bool checkExtSupport(VkPhysicalDevice device);
  SwapchainSupportDetails getSwapchainSupport(VkPhysicalDevice device,
                                              VkSurfaceKHR surface);
  void pickPhysicalDevice(VkSurfaceKHR surface);
  void createLogicalDevice(VkSurfaceKHR surface);

public:
  Device(VkInstance instance, VkSurfaceKHR surface);
  ~Device() { this->dispose(); }

  QueueFamilyIndices getQueue() const { return mQueueFamily; }
  SwapchainSupportDetails getSwapchainSupport(VkSurfaceKHR surface) {
    return this->getSwapchainSupport(mPhysicalDevice, surface);
  }

  void dispose();

  operator VkDevice() const { return mDevice; }
  operator VkPhysicalDevice() const { return mPhysicalDevice; }
};
} // namespace Terreate::Core
