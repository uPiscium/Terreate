#include "vulkan/device.hpp"

#include <iostream>

namespace Terreate::Vulkan {

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice,
                               shared<SDL::Window> window)
    : mWindow(window), mPhysicalDevice(physicalDevice) {
  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount,
                                           nullptr);
  mQueueFamilies.resize(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount,
                                           mQueueFamilies.data());

  u32 extensionCount = 0;
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr,
                                       &extensionCount, nullptr);
  mSupportedExtensions.resize(extensionCount);
  vkEnumerateDeviceExtensionProperties(
      mPhysicalDevice, nullptr, &extensionCount, mSupportedExtensions.data());

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, *window,
                                            &mSurfaceCapabilities);

  u32 formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, *window, &formatCount,
                                       nullptr);
  if (formatCount != 0) {
    mSurfaceFormats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, *window, &formatCount,
                                         mSurfaceFormats.data());
  }

  u32 presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, *window,
                                            &presentModeCount, nullptr);
  if (presentModeCount != 0) {
    mPresentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        mPhysicalDevice, *window, &presentModeCount, mPresentModes.data());
  }

  vkGetPhysicalDeviceProperties(mPhysicalDevice, &mProperties);
  vkGetPhysicalDeviceFeatures(mPhysicalDevice, &mSupportedFeatures);
}

uset<u32> PhysicalDevice::getQueue(VkQueueFlags const &flags) const {
  uset<u32> indices;
  for (u32 i = 0; i < mQueueFamilies.size(); ++i) {
    auto const &queueFamily = mQueueFamilies[i];
    if (queueFamily.queueCount <= 0) {
      continue;
    }

    if ((queueFamily.queueFlags & flags) == flags) {
      indices.insert(i);
    }
  }

  return indices;
}

vec<VkQueueFamilyProperties> const &PhysicalDevice::getQueueFamilies() const {
  return mQueueFamilies;
}

vec<VkExtensionProperties> const &
PhysicalDevice::getSupportedExtensions() const {
  return mSupportedExtensions;
}

VkSurfaceCapabilitiesKHR const &PhysicalDevice::getSurfaceCapabilities() const {
  return mSurfaceCapabilities;
}

vec<VkSurfaceFormatKHR> const &PhysicalDevice::getSurfaceFormats() const {
  return mSurfaceFormats;
}

vec<VkPresentModeKHR> const &PhysicalDevice::getPresentModes() const {
  return mPresentModes;
}

VkPhysicalDeviceProperties const &PhysicalDevice::getProperties() const {
  return mProperties;
}

VkPhysicalDeviceFeatures const &PhysicalDevice::getSupportedFeatures() const {
  return mSupportedFeatures;
}

bool PhysicalDevice::hasQueueSupport(
    uset<QueueType> const &requiredQueues) const {
  for (auto const &queue : requiredQueues) {
    if (this->getQueue((VkQueueFlags)queue).empty()) {
      return false;
    }
  }
  return true;
}

bool PhysicalDevice::hasSurfaceSupport(u32 queueFamilyIndex) const {
  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, queueFamilyIndex,
                                       *mWindow, &presentSupport);
  return presentSupport;
}

bool PhysicalDevice::hasExtensionSupport(str const &extensionName) const {
  for (auto const &extension : mSupportedExtensions) {
    if (extensionName == extension.extensionName) {
      return true;
    }
  }
  return false;
}

bool PhysicalDevice::hasExtensionSupport(vec<str> const &extensionNames) const {
  for (auto const &extensionName : extensionNames) {
    if (!this->hasExtensionSupport(extensionName)) {
      return false;
    }
  }
  return true;
}

bool PhysicalDevice::hasSwapChainSupport() const {
  return !mSurfaceFormats.empty() && !mPresentModes.empty();
}

PhysicalDevice::operator VkPhysicalDevice() const { return mPhysicalDevice; }

shared<PhysicalDevice> PhysicalDevice::create(VkPhysicalDevice physicalDevice,
                                              shared<SDL::Window> window) {
  PhysicalDevice *device = new PhysicalDevice(physicalDevice, window);
  return shared<PhysicalDevice>(device);
}

i32 PhysicalDevice::rateDevice(shared<PhysicalDevice> physicalDevice) {
  int score = 0;
  VkPhysicalDeviceProperties properties = physicalDevice->getProperties();

  // Discrete GPUs have a significant performance advantage
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  score += properties.limits.maxImageDimension2D;
  score += properties.limits.maxImageDimensionCube;
  score += properties.limits.maxImageArrayLayers;
  score += properties.limits.maxMemoryAllocationCount;
  score += properties.limits.maxComputeWorkGroupInvocations;
  score += properties.limits.maxComputeSharedMemorySize;

  return score;
}

shared<PhysicalDevice> PhysicalDevice::pickPhysicalDevice(
    shared<Instance> instance, shared<SDL::Window> window,
    uset<QueueType> const &requiredQueues, vec<str> const &requiredExtensions,
    DeviceRateFunction func) {
  u32 deviceCount = 0;
  vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw DeviceException("Failed to find GPUs with Vulkan support.");
  }
  vec<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

  i32 bestScore = -1;
  shared<PhysicalDevice> bestDevice = nullptr;
  for (auto const &device : devices) {
    shared<PhysicalDevice> physicalDevice =
        PhysicalDevice::create(device, window);

    if (!physicalDevice->hasQueueSupport(requiredQueues)) {
      continue;
    }

    if (!physicalDevice->hasExtensionSupport(requiredExtensions)) {
      continue;
    }

    if (requiredQueues.contains(QueueType::GRAPHICS)) {
      uset<u32> graphicsQueueIndex =
          physicalDevice->getQueue((VkQueueFlags)QueueType::GRAPHICS);

      bool hasSurfaceSupport = false;
      for (u32 const &index : graphicsQueueIndex) {
        if (physicalDevice->hasSurfaceSupport(index)) {
          hasSurfaceSupport = true;
          break;
        }
      }
      if (!hasSurfaceSupport) {
        continue;
      }

      if (!physicalDevice->hasSwapChainSupport()) {
        continue;
      }
    }

    i32 deviceScore = func(physicalDevice);
    if (deviceScore > bestScore) {
      bestScore = deviceScore;
      bestDevice = physicalDevice;
    }
  }

  if (bestDevice == nullptr) {
    throw DeviceException("Failed to find a suitable GPU.");
  }

  return bestDevice;
}

Device::Device(shared<Instance> instance, shared<SDL::Window> window,
               shared<PhysicalDevice> physicalDevice,
               uset<QueueType> const &requiredQueues,
               vec<str> const &requiredExtensions)
    : mInstance(instance), mPhysicalDevice(physicalDevice) {
  float queuePriority = 1.0f;
  vec<VkDeviceQueueCreateInfo> queueCreateInfos;

  for (auto const &queue : requiredQueues) {
    uset<u32> indices = physicalDevice->getQueue((VkQueueFlags)queue);
    if (indices.empty()) {
      throw DeviceException("Failed to find a suitable queue family.");
    }

    for (auto const &index : indices) {
      bool found = true;
      for (auto const &pair : mQueueFamilyIndices) {
        if (pair.second == index) {
          found = false;
          break;
        }
      }

      if (found) {
        mQueueFamilyIndices.insert({queue, index});
        break;
      }
    }
  }

  for (auto const &pair : mQueueFamilyIndices) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = pair.second;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = queueCreateInfos.size();
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = requiredExtensions.size();
  createInfo.ppEnabledExtensionNames =
      (char const *const *)requiredExtensions.data();

  vec<str> layers = mInstance->getEnabledLayers();
  if (mInstance->isDebugMode()) {
    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = (char const *const *)layers.data();
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  if (vkCreateDevice(*physicalDevice, &createInfo, nullptr, &mDevice) !=
      VK_SUCCESS) {
    throw DeviceException("Failed to create logical device.");
  }
}

Device::~Device() {
  if (mDevice != VK_NULL_HANDLE) {
    vkDestroyDevice(mDevice, nullptr);
    mDevice = VK_NULL_HANDLE;
  }
}

umap<QueueType, i32> const &Device::getQueueFamilyIndices() const {
  return mQueueFamilyIndices;
}

vec<VkQueueFamilyProperties> const &Device::getQueueFamilies() const {
  return mPhysicalDevice->getQueueFamilies();
}

vec<VkExtensionProperties> const &Device::getSupportedExtensions() const {
  return mPhysicalDevice->getSupportedExtensions();
}

VkSurfaceCapabilitiesKHR const &Device::getSurfaceCapabilities() const {
  return mPhysicalDevice->getSurfaceCapabilities();
}

vec<VkSurfaceFormatKHR> const &Device::getSurfaceFormats() const {
  return mPhysicalDevice->getSurfaceFormats();
}

vec<VkPresentModeKHR> const &Device::getPresentModes() const {
  return mPhysicalDevice->getPresentModes();
}

VkPhysicalDeviceProperties const &Device::getProperties() const {
  return mPhysicalDevice->getProperties();
}

VkPhysicalDeviceFeatures const &Device::getSupportedFeatures() const {
  return mPhysicalDevice->getSupportedFeatures();
}

Device::operator VkDevice() const { return mDevice; }

shared<Device> Device::create(shared<Instance> instance,
                              shared<SDL::Window> window,
                              uset<QueueType> const &requiredQueues,
                              vec<str> const &requiredExtensions) {
  shared<PhysicalDevice> physicalDevice = PhysicalDevice::pickPhysicalDevice(
      instance, window, requiredQueues, requiredExtensions);
  Device *device = new Device(instance, window, physicalDevice, requiredQueues,
                              requiredExtensions);
  return shared<Device>(device);
}

shared<Device> Device::create(shared<Instance> instance,
                              shared<SDL::Window> window,
                              shared<PhysicalDevice> physicalDevice,
                              uset<QueueType> const &requiredQueues,
                              vec<str> const &requiredExtensions) {
  Device *device = new Device(instance, window, physicalDevice, requiredQueues,
                              requiredExtensions);
  return shared<Device>(device);
}

} // namespace Terreate::Vulkan
