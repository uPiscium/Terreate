#include <iostream>

#include <main.hpp>

#ifdef TERREATE_DEBUG_BUILD

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanTriangle::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
  std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

bool VulkanTriangle::checkValidationLayerSupport() {
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
      std::cerr << "Validation layer " << layerName << " not found!"
                << std::endl;
      return false;
    }
  }

  return true;
}

void VulkanTriangle::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
}

void VulkanTriangle::setupDebugMessenger() {
  if (!this->checkValidationLayerSupport()) {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  this->populateDebugMessengerCreateInfo(createInfo);
  if (trCreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr,
                                     &mDebugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("Failed to set up debug messenger!");
  }
}

#endif
