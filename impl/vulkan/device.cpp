#include "../../include/common/exception.hpp"
#include "../../include/vulkan/device.hpp"
#include "../../include/vulkan/vk.hpp"

namespace Terreate::Vulkan {
QueueFamilyIndices Device::findQueue(VkPhysicalDevice device,
                                     VkObjectRef<ISurface> surface) const {
  QueueFamilyIndices queueFamily;

  Type::u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  Type::vec<VkQueueFamilyProperties> queueProperties(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueProperties.data());

  Type::u32 i = 0;
  for (auto const &property : queueProperties) {
    if (property.queueCount > 0 &&
        (property.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
      queueFamily.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentSupport);
    if (presentSupport) {
      queueFamily.presentFamily = i;
    }

    if (queueFamily) {
      break;
    }

    ++i;
  }

  if (!queueFamily) {
    Type::str msg = "No suitable queue family found for the device";
    throw Exception::NoQueueFamilyFound(msg);
  }

  return queueFamily;
}

int Device::rateDevice(VkPhysicalDevice device,
                       VkObjectRef<ISurface> surface) const {
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);

  int score = SCORES.at(deviceProperties.deviceType);

  QueueFamilyIndices indices = this->findQueue(device, surface);
  if (!indices) {
    return 0;
  }

  score += deviceProperties.limits.maxImageDimension2D;

  return score;
}

bool Device::checkExtSupport(VkPhysicalDevice device) {
  Type::u32 extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);
  Type::vec<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  Type::set<Type::str> requiredExtensions(EXTS.begin(), EXTS.end());

  for (auto const &exts : availableExtensions) {
    requiredExtensions.erase(exts.extensionName);
  }

  return requiredExtensions.empty();
}

SwapchainSupportDetails
Device::getSwapchainSupport(VkPhysicalDevice device,
                            VkObjectRef<ISurface> surface) {
  SwapchainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *surface,
                                            &details.capabilities);
  Type::u32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surface, &formatCount,
                                         details.formats.data());
  }

  Type::u32 presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, *surface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, *surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

void Device::pickPhysicalDevice(VkObjectRef<ISurface> surface) {
  Type::map<Type::i32, VkPhysicalDevice> deviceMap;
  Type::u32 deviceCount = 0;
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    Type::str msg = "No GPU with Vulkan support found";
    throw Exception::NoDeviceFound(msg);
  }
  Type::vec<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

  for (auto &device : devices) {
    if (!this->checkExtSupport(device)) {
      continue;
    }

    auto score = this->rateDevice(device, surface);
    if (score <= 0) {
      continue;
    }

    SwapchainSupportDetails swapchainSupport =
        this->getSwapchainSupport(device, surface);
    if (swapchainSupport.formats.empty() ||
        swapchainSupport.presentModes.empty()) {
      continue;
    }

    deviceMap.insert(std::make_pair(score, device));
  }

  if (deviceMap.empty()) {
    Type::str msg = "No suitable GPU found";
    throw Exception::NoDeviceFound(msg);
  }

  mPhysicalDevice = deviceMap.rbegin()->second;
  mQueueFamily = this->findQueue(mPhysicalDevice, surface);
}

void Device::createLogicalDevice(VkObjectRef<ISurface> surface) {
  Type::vec<VkDeviceQueueCreateInfo> queueCreateInfos;
  float queuePriority = 1.0f;

  for (auto const &family : Type::set<Type::u32>(mQueueFamily)) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = family;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount =
      static_cast<Type::u32>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = static_cast<Type::u32>(EXTS.size());
  createInfo.ppEnabledExtensionNames = EXTS.data();

#ifdef TERREATE_DEBUG_BUILD
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#else
  createInfo.enabledLayerCount = 0;
#endif

  if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) !=
      VK_SUCCESS) {
    Type::str msg = "Failed to create logical device";
    throw Exception::DeviceCreationFailure(msg);
  }
}

Device::Device(VkInstance instance, VkObjectRef<ISurface> surface)
    : mInstance(instance) {
  this->pickPhysicalDevice(surface);
  this->createLogicalDevice(surface);
}

Device::~Device() {
  if (mDevice != VK_NULL_HANDLE) {
    vkDestroyDevice(mDevice, nullptr);
    mDevice = VK_NULL_HANDLE;
  }
}
} // namespace Terreate::Vulkan
