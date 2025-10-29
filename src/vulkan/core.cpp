#include "vulkan/core.hpp"
#include "vulkan/exception.hpp"

namespace Terreate::Vulkan {
bool Instance::checkValidationLayerSupport(vec<str> const &layers) const {
  u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  vec<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (auto const &layer : layers) {
    bool layerFound = false;

    for (const auto &availableLayer : availableLayers) {
      if (std::strcmp(layer.c_str(), availableLayer.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

vec<char const *> Instance::getRequiredExtensions(vec<str> const &exts) const {
  u32 extensionCount = 0;
  char const *const *extensions =
      SDL_Vulkan_GetInstanceExtensions(&extensionCount);
  if (!extensions) {
    throw std::runtime_error("Failed to get required Vulkan extensions.");
  }
  vec<char const *> requiredExtensions(extensions, extensions + extensionCount);

  for (auto const &ext : exts) {
    requiredExtensions.push_back(ext.c_str());
  }

  if (mDebugMode) {
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return requiredExtensions;
}

VkDebugUtilsMessengerCreateInfoEXT
Instance::getDebugMessengerCreateInfo() const {
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallbackWrapper;
  createInfo.pUserData = nullptr; // Optional
  createInfo.flags = 0;

  return createInfo;
}

Instance::Instance(str const &appName, Version const &appVersion,
                   bool debugMode, vec<str> const &extensions,
                   vec<str> const &layers)
    : mDebugMode(debugMode) {
  if (mDebugMode && !this->checkValidationLayerSupport(layers)) {
    throw InstanceException("Validation layers requested, but not available.");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName.c_str();
  appInfo.applicationVersion = makeVersion(appVersion);
  appInfo.pEngineName = ENGINE_NAME;
  appInfo.engineVersion = makeVersion(ENGINE_VERSION);
  appInfo.apiVersion = VK_API_VERSION_1_4;

  vec<char const *> requiredExtensions =
      this->getRequiredExtensions(extensions);
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount =
      static_cast<u32>(requiredExtensions.size());
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo =
      this->getDebugMessengerCreateInfo();
  if (mDebugMode) {
    createInfo.enabledLayerCount = static_cast<u32>(layers.size());
    vec<char const *> layerNames;
    for (auto const &layer : layers) {
      layerNames.push_back(layer.c_str());
    }
    createInfo.ppEnabledLayerNames = layerNames.data();
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &mHandle) != VK_SUCCESS) {
    throw InstanceException("Failed to create Vulkan instance.");
  }
}

Instance::~Instance() {
  if (mDebugMessenger != VK_NULL_HANDLE) {
    destroyDebugUtilsMessengerEXT(mHandle, mDebugMessenger, nullptr);
    mDebugMessenger = VK_NULL_HANDLE;
  }

  if (mHandle != VK_NULL_HANDLE) {
    vkDestroyInstance(mHandle, nullptr);
    mHandle = VK_NULL_HANDLE;
  }
}

void Instance::attachDebugger(shared<IDebugger> debugger) {
  if (!debugger || !mDebugMode) {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo =
      this->getDebugMessengerCreateInfo();
  debugCreateInfo.pUserData = debugger.get();
  mDebugMessenger =
      createDebugUtilsMessengerEXT(mHandle, &debugCreateInfo, nullptr);
}

} // namespace Terreate::Vulkan
