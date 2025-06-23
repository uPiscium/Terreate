#pragma once
#include "common/enum.hpp"
#include "common/type.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace Terreate::TestImpl {
Type::u32 constexpr WIDTH = 800;
Type::u32 constexpr HEIGHT = 600;
Type::vec<char const *> const VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"};

#ifdef TERREATE_DEBUG_BUILD
bool constexpr ENABLE_DEBUG_BUILD = true;
#else
bool constexpr ENABLE_DEBUG_BUILD = false;
#endif

class VulkanTriangle {
private:
  GLFWwindow *mWindow = nullptr;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
  VkInstance mInstance = VK_NULL_HANDLE;

private:
  GLFWwindow *initWindow(int width = WIDTH, int height = HEIGHT,
                         Type::str const &title = "Vulkan Triangle");
  void destroyWindow(GLFWwindow *window);

  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo,
      VkDebugUtilsMessageSeverityFlagsEXT messageSeverity =
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      VkDebugUtilsMessageTypeFlagsEXT messageType =
          VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
      PFN_vkDebugUtilsMessengerCallbackEXT pfnCallback = debugCallback);
  VkDebugUtilsMessengerEXT
  createDebugMessenger(VkInstance instance,
                       VkDebugUtilsMessengerCreateInfoEXT const &createInfo);
  void destroyDebugMessenger(VkInstance instance,
                             VkDebugUtilsMessengerEXT debugMessenger);

  bool checkValidationLayerSupport();
  Type::vec<char const *> getRequiredExtensions();
  VkInstance createInstance(Type::str const &appName = "Vulkan Triangle",
                            Type::str const &engineName = "Terreate Engine");
  void destroyInstance(VkInstance instance);

  void initVulkan();
  void mainLoop();
  void cleanup();

public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                void *pUserData);

public:
  void run();
};
} // namespace Terreate::TestImpl

int main();
