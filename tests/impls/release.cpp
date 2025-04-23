#include <iostream>

#include <main.hpp>

#ifndef TERREATE_DEBUG_BUILD

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanTriangle::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
  return VK_FALSE;
}
bool VulkanTriangle::checkValidationLayerSupport() { return false; }
void VulkanTriangle::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
}
void VulkanTriangle::setupDebugMessenger() {}

#endif
