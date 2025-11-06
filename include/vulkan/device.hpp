#pragma once

#include "common/type.hpp"

#include "sdl/window.hpp"

#include "enum.hpp"
#include "instance.hpp"

namespace Terreate::Vulkan {

class PhysicalDevice;
typedef function<i32(shared<PhysicalDevice>)> DeviceRateFunction;

class PhysicalDevice {
private:
  shared<SDL::Window> mWindow = nullptr;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  vec<VkQueueFamilyProperties> mQueueFamilies;
  vec<VkExtensionProperties> mSupportedExtensions;
  VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
  vec<VkSurfaceFormatKHR> mSurfaceFormats;
  vec<VkPresentModeKHR> mPresentModes;
  VkPhysicalDeviceProperties mProperties;
  VkPhysicalDeviceFeatures mSupportedFeatures;

private:
  PhysicalDevice(VkPhysicalDevice physicalDevice, shared<SDL::Window> window);

public:
  ~PhysicalDevice() = default;

  uset<u32> getQueue(VkQueueFlags const &flags) const;
  vec<VkQueueFamilyProperties> const &getQueueFamilies() const;
  vec<VkExtensionProperties> const &getSupportedExtensions() const;
  VkSurfaceCapabilitiesKHR const &getSurfaceCapabilities() const;
  vec<VkSurfaceFormatKHR> const &getSurfaceFormats() const;
  vec<VkPresentModeKHR> const &getPresentModes() const;
  VkPhysicalDeviceProperties const &getProperties() const;
  VkPhysicalDeviceFeatures const &getSupportedFeatures() const;

  bool hasQueueSupport(uset<QueueType> const &requiredQueues) const;
  bool hasSurfaceSupport(u32 queueFamilyIndex) const;
  bool hasExtensionSupport(str const &extensionName) const;
  bool hasExtensionSupport(vec<str> const &extensionName) const;
  bool hasSwapChainSupport() const;

  operator VkPhysicalDevice() const;

public:
  static shared<PhysicalDevice> create(VkPhysicalDevice physicalDevice,
                                       shared<SDL::Window> window);
  static i32 rateDevice(shared<PhysicalDevice> physicalDevice);
  static shared<PhysicalDevice>
  pickPhysicalDevice(shared<Instance> instance, shared<SDL::Window> window,
                     uset<QueueType> const &requiredQueues,
                     vec<str> const &requiredExtensions = {},
                     DeviceRateFunction func = PhysicalDevice::rateDevice);
};

class Device {
private:
  shared<Instance> mInstance = nullptr;
  shared<PhysicalDevice> mPhysicalDevice = nullptr;
  VkDevice mDevice = VK_NULL_HANDLE;
  umap<QueueType, i32> mQueueFamilyIndices;

private:
  Device(shared<Instance> instance, shared<SDL::Window> window,
         shared<PhysicalDevice> physicalDevice,
         uset<QueueType> const &requiredQueues,
         vec<str> const &requiredExtensions);

public:
  ~Device();

  umap<QueueType, i32> const &getQueueFamilyIndices() const;
  vec<VkQueueFamilyProperties> const &getQueueFamilies() const;
  vec<VkExtensionProperties> const &getSupportedExtensions() const;
  VkSurfaceCapabilitiesKHR const &getSurfaceCapabilities() const;
  vec<VkSurfaceFormatKHR> const &getSurfaceFormats() const;
  vec<VkPresentModeKHR> const &getPresentModes() const;
  VkPhysicalDeviceProperties const &getProperties() const;
  VkPhysicalDeviceFeatures const &getSupportedFeatures() const;

  operator VkDevice() const;

public:
  static shared<Device> create(
      shared<Instance> instance, shared<SDL::Window> window,
      uset<QueueType> const &requiredQueues = {QueueType::GRAPHICS,
                                               QueueType::COMPUTE},
      vec<str> const &requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME});
  static shared<Device> create(
      shared<Instance> instance, shared<SDL::Window> window,
      shared<PhysicalDevice> physicalDevice,
      uset<QueueType> const &requiredQueues = {QueueType::GRAPHICS,
                                               QueueType::COMPUTE},
      vec<str> const &requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME});
};

} // namespace Terreate::Vulkan
