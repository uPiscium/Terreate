#include <core/api.hpp>
#include <exception.hpp>

namespace Terreate::API {
void loadEXTfunctions(VkInstance instance) {
#ifdef TERREATE_DEBUG_BUILD
  trCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkCreateDebugUtilsMessengerEXT");
  trDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkDestroyDebugUtilsMessengerEXT");

  if (!trCreateDebugUtilsMessengerEXT || !trDestroyDebugUtilsMessengerEXT) {
    throw Exception::DebugMessengerFunctionLoadFailure();
  }
#endif
}

vec<char const *> getRequiredExts() {
  uint32_t glfwExtensionCount = 0;
  char const **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<char const *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
#ifdef TERREATE_DEBUG_BUILD
  extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  return extensions;
}

bool checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (char const *layerName : VALIDATION_LAYERS) {
    bool layerFound = false;

    for (auto const &layerProp : availableLayers) {
      if (strcmp(layerName, layerProp.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      throw Exception::ValidationLayerNotSupported();
      return false;
    }
  }

  return true;
}

VkApplicationInfo createAppInfo(char const *appName, Version appVersion,
                                char const *engineName, Version engineVersion,
                                u32 apiVersion) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName;
  appInfo.applicationVersion =
      VK_MAKE_VERSION(appVersion.major, appVersion.minor, appVersion.patch);
  appInfo.pEngineName = engineName;
  appInfo.engineVersion = VK_MAKE_VERSION(
      engineVersion.major, engineVersion.minor, engineVersion.patch);
  appInfo.apiVersion = apiVersion;
  return appInfo;
}

VkDebugUtilsMessengerCreateInfoEXT
createDebugCreateInfo(PFN_vkDebugUtilsMessengerCallbackEXT callback,
                      void *userData) {
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = callback;
  createInfo.pUserData = userData;
  createInfo.pNext = nullptr;

  return createInfo;
}

VkInstance createInstance(VkApplicationInfo const &appInfo) {
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
  createInfo.pNext = nullptr;

  auto exts = getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw Exception::InstanceCreationFailure();
  }
  loadEXTfunctions(instance);

  return instance;
}

VkInstance
createInstance(VkApplicationInfo const &appInfo,
               VkDebugUtilsMessengerCreateInfoEXT const &debuggerInfo) {
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debuggerInfo;

  auto exts = getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw Exception::InstanceCreationFailure();
  }
  loadEXTfunctions(instance);

  return instance;
}

bool isDeviceExtsSupported(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  set<str> requiredExtensions(EXTS.begin(), EXTS.end());

  for (auto const &ext : availableExtensions) {
    requiredExtensions.erase(ext.extensionName);
  }

  return requiredExtensions.empty();
}

vec<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());
  return queueFamilies;
}

SwapchainSupport getSwapchainSupport(VkPhysicalDevice device,
                                     VkSurfaceKHR surface) {
  SwapchainSupport details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

vec<VkPhysicalDevice> getPhysicalDevices(VkInstance instance) {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw Exception::NoDeviceFound();
  }
  vec<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
  return devices;
}

vec<VkImage> getSwapchainImages(VkDevice device, VkSwapchainKHR swapchain) {
  uint32_t swapchainImgCount;
  vkGetSwapchainImagesKHR(device, swapchain, &swapchainImgCount, nullptr);
  vec<VkImage> swapchainImages(swapchainImgCount);
  vkGetSwapchainImagesKHR(device, swapchain, &swapchainImgCount,
                          swapchainImages.data());
  return swapchainImages;
}

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format) {
  VkImageViewCreateInfo createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.image = image;
  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = format;

  createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;

  createInfo.pNext = nullptr;

  VkImageView imgView;
  if (vkCreateImageView(device, &createInfo, nullptr, &imgView) != VK_SUCCESS) {
    throw Exception::ImageViewCreationFailure();
  }

  return imgView;
}

VkShaderModule createShaderModule(VkDevice device, vec<i8> const &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<uint32_t const *>(code.data());

  VkShaderModule module;
  if (vkCreateShaderModule(device, &createInfo, nullptr, &module) !=
      VK_SUCCESS) {
    throw Exception::ShaderModuleCreationFailure();
  }

  return module;
}
} // namespace Terreate::API
