#include "../include/main.hpp"

namespace Terreate::TestImpl {
bool VulkanTriangle::checkValidationLayerSupport() {
  Type::u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  if (layerCount == 0) {
    return false; // No validation layers available
  }

  Type::vec<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (auto const &layer : availableLayers) {
    bool found = false;
    for (auto const &validLayer : VALIDATION_LAYERS) {
      if (strcmp(layer.layerName, validLayer) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      return false; // A required validation layer is not available
    }
  }
  return true; // All required validation layers are available
}

Type::vec<char const *> VulkanTriangle::getRequiredExtensions() {
  Type::vec<char const *> extensions;
  Type::u32 extensionCount = 0;
  char const **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&extensionCount);
  for (Type::u32 i = 0; i < extensionCount; ++i) {
    extensions.push_back(glfwExtensions[i]);
  }

  if (ENABLE_DEBUG_BUILD && checkValidationLayerSupport()) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

VkInstance VulkanTriangle::createInstance(Type::str const &appName,
                                          Type::str const &engineName) {
  if (ENABLE_DEBUG_BUILD && !checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName.c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = engineName.c_str();
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto exts = this->getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<Type::u32>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

  if (ENABLE_DEBUG_BUILD && checkValidationLayerSupport()) {
    createInfo.enabledLayerCount =
        static_cast<Type::u32>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (ENABLE_DEBUG_BUILD) {
    this->populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = &debugCreateInfo;
  } else {
    createInfo.pNext = nullptr;
  }

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan instance");
  }

  return instance;
}

void VulkanTriangle::destroyInstance(VkInstance instance) {
  if (instance != VK_NULL_HANDLE) {
    vkDestroyInstance(instance, nullptr);
  }
  mInstance = VK_NULL_HANDLE;
}

} // namespace Terreate::TestImpl
