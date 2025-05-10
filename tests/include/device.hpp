#pragma once

#include <api.hpp>
#include <exception.hpp>
#include <type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

struct QueueFamilyIndices {
public:
  optional<u32> graphicsFamily;
  optional<u32> presentFamily;

  operator set<u32>() const;
  operator bool() const;
};

class LogicalDevice {
private:
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;
  VkQueue mGraphicsQueue = VK_NULL_HANDLE;
  VkQueue mPresentQueue = VK_NULL_HANDLE;

private:
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                       VkSurfaceKHR surface) const;
  int rateDevice(VkPhysicalDevice device, VkSurfaceKHR surface) const;
  void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

public:
  LogicalDevice(VkInstance instance, VkSurfaceKHR surface);
  ~LogicalDevice();

  QueueFamilyIndices getQueueFamilies(VkSurfaceKHR surface) const;

  operator VkPhysicalDevice() const;
  operator VkDevice() const;
};
} // namespace Terreate::Core
