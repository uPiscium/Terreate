#pragma once

#include "common/type.hpp"

#include "core/sdl/window.hpp"

#include "enum.hpp"
#include "instance.hpp"

namespace Terreate::Core::Vulkan {

class PhysicalDevice;
typedef function<i32(shared<PhysicalDevice>)> DeviceRateFunction;

class PhysicalDevice {
private:
  PROHIBIT_COPY_AND_ASSIGN(PhysicalDevice);

private:
  shared<SDL::Window> mWindow = nullptr;
  VkPhysicalDevice mHandle = VK_NULL_HANDLE;
  VkSampleCountFlagBits mMaxSampleCount = VK_SAMPLE_COUNT_1_BIT;
  vec<VkQueueFamilyProperties> mQueueFamilies;
  vec<VkExtensionProperties> mSupportedExtensions;
  VkPhysicalDeviceProperties mProperties;
  VkPhysicalDeviceFeatures mSupportedFeatures;
  VkPhysicalDeviceMemoryProperties mMemoryProperties;

private:
  VkSampleCountFlagBits getMaxUsableSampleCount() const;
  PhysicalDevice(VkPhysicalDevice physicalDevice,
                 shared<SDL::Window> const &window);

public:
  ~PhysicalDevice() = default;

  VkSampleCountFlagBits const &getMaxSampleCount() const;
  uset<u32> getQueue(VkQueueFlags const &flags) const;
  vec<VkQueueFamilyProperties> const &getQueueFamilies() const;
  vec<VkExtensionProperties> const &getSupportedExtensions() const;
  VkPhysicalDeviceProperties const &getProperties() const;
  VkPhysicalDeviceFeatures const &getSupportedFeatures() const;
  VkPhysicalDeviceMemoryProperties const &getMemoryProperties() const;
  VkFormatProperties getFormatProperties(VkFormat const &format) const;
  VkSurfaceCapabilitiesKHR getSurfaceCapabilities() const;
  vec<VkSurfaceFormatKHR> getSurfaceFormats() const;
  vec<VkPresentModeKHR> getPresentModes() const;

  u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;
  bool hasQueueSupport(uset<QueueType> const &requiredQueues) const;
  bool hasSurfaceSupport(u32 queueFamilyIndex) const;
  bool hasExtensionSupport(str const &extensionName) const;
  bool hasExtensionSupport(vec<str> const &extensionName) const;
  bool hasSwapChainSupport() const;

  operator VkPhysicalDevice() const;

public:
  static shared<PhysicalDevice> create(VkPhysicalDevice physicalDevice,
                                       shared<SDL::Window> const &window);
  static i32 rateDevice(shared<PhysicalDevice> const &physicalDevice);
  static shared<PhysicalDevice>
  pick(shared<Instance> const &instance, shared<SDL::Window> const &window,
       uset<QueueType> const &requiredQueues,
       vec<str> const &requiredExtensions = {},
       DeviceRateFunction func = PhysicalDevice::rateDevice);
};

class Device {
private:
  shared<Instance> mInstance = nullptr;
  shared<PhysicalDevice> mPhysicalDevice = nullptr;
  VkDevice mHandle = VK_NULL_HANDLE;
  umap<QueueType, u32> mQueueFamilyIndices;

private:
  Device(shared<Instance> const &instance, shared<SDL::Window> const &window,
         shared<PhysicalDevice> const &physicalDevice,
         vec<DeviceFeatures> const &requiredFeatures,
         uset<QueueType> const &requiredQueues,
         vec<str> const &requiredExtensions);

public:
  ~Device();

  shared<PhysicalDevice> const &getPhysicalDevice() const;
  umap<QueueType, u32> const &getQueueFamilyIndices() const;
  VkSampleCountFlagBits const &getMaxSampleCount() const;
  vec<VkQueueFamilyProperties> const &getQueueFamilies() const;
  vec<VkExtensionProperties> const &getSupportedExtensions() const;
  VkPhysicalDeviceProperties const &getProperties() const;
  VkPhysicalDeviceFeatures const &getSupportedFeatures() const;
  VkPhysicalDeviceMemoryProperties const &getMemoryProperties() const;
  VkFormatProperties getFormatProperties(VkFormat const &format) const;
  VkSurfaceCapabilitiesKHR getSurfaceCapabilities() const;
  vec<VkSurfaceFormatKHR> getSurfaceFormats() const;
  vec<VkPresentModeKHR> getPresentModes() const;

  u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;
  void waitIdle() const;

  operator VkDevice() const;

public:
  static shared<Device> create(
      shared<Instance> const &instance, shared<SDL::Window> const &window,
      vec<DeviceFeatures> const &requiredFeatures =
          {Vulkan::DeviceFeatures::SAMPLER_ANISOTROPY,
           Vulkan::DeviceFeatures::SAMPLE_RATE_SHADING},
      uset<QueueType> const &requiredQueues = {QueueType::GRAPHICS,
                                               QueueType::COMPUTE},
      vec<str> const &requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME});
  static shared<Device> create(
      shared<Instance> const &instance, shared<SDL::Window> const &window,
      shared<PhysicalDevice> const &physicalDevice,
      vec<DeviceFeatures> const &requiredFeatures =
          {Vulkan::DeviceFeatures::SAMPLER_ANISOTROPY,
           Vulkan::DeviceFeatures::SAMPLE_RATE_SHADING},
      uset<QueueType> const &requiredQueues = {QueueType::GRAPHICS,
                                               QueueType::COMPUTE},
      vec<str> const &requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME});
};

} // namespace Terreate::Core::Vulkan
