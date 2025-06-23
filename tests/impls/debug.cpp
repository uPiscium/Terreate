#include "../include/main.hpp"

namespace Terreate::TestImpl {
void VulkanTriangle::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo,
    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    PFN_vkDebugUtilsMessengerCallbackEXT pfnCallback) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = messageSeverity;
  createInfo.messageType = messageType;
  createInfo.pfnUserCallback = pfnCallback;
  createInfo.pUserData = nullptr; // Optional
}

VkDebugUtilsMessengerEXT VulkanTriangle::createDebugMessenger(
    VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const &createInfo) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    VkDebugUtilsMessengerEXT debugMessenger;
    if (func(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create debug messenger!");
    }
    return debugMessenger;
  } else {
    throw std::runtime_error("Could not find vkCreateDebugUtilsMessengerEXT!");
  }
}

void VulkanTriangle::destroyDebugMessenger(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger) {
  if (debugMessenger != VK_NULL_HANDLE) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(instance, debugMessenger, nullptr);
    } else {
      throw std::runtime_error(
          "Could not find vkDestroyDebugUtilsMessengerEXT!");
    }
  }
  mDebugMessenger = VK_NULL_HANDLE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanTriangle::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  }
  return VK_FALSE;
}
} // namespace Terreate::TestImpl
