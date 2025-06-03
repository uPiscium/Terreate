// #pragma once

// #define GLFW_INCLUDE_NONE
// #define GLFW_INCLUDE_VULKAN
// #include <glfw/glfw3.h>

// #include <debugger.hpp>
// #include <type.hpp>

// namespace Terreate::API {
// using namespace Terreate::Type;

// static char const *VK_LAYER_KHRONOS_VALIDATION =
// "VK_LAYER_KHRONOS_validation"; static std::vector<char const *> const
// VALIDATION_LAYERS = {
//     VK_LAYER_KHRONOS_VALIDATION};
// static umap<VkPhysicalDeviceType, int> const SCORES = {
//     {VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, 10000},
//     {VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, 1000},
//     {VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU, 100},
//     {VK_PHYSICAL_DEVICE_TYPE_CPU, 10},
//     {VK_PHYSICAL_DEVICE_TYPE_OTHER, 0}};
// static vec<char const *> const EXTS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

// inline PFN_vkCreateDebugUtilsMessengerEXT trCreateDebugUtilsMessengerEXT =
//     nullptr;
// inline PFN_vkDestroyDebugUtilsMessengerEXT trDestroyDebugUtilsMessengerEXT =
//     nullptr;

// struct SwapchainSupport {
//   VkSurfaceCapabilitiesKHR capabilities;
//   std::vector<VkSurfaceFormatKHR> formats;
//   std::vector<VkPresentModeKHR> presentModes;
// };

// void loadEXTfunctions(VkInstance instance);
// vec<char const *> getRequiredExts();
// bool checkValidationLayerSupport();
// VkApplicationInfo createAppInfo(char const *appName, Version appVersion,
//                                 char const *engineName = "No Engine",
//                                 Version engineVersion = {1, 0, 0},
//                                 u32 apiVersion = VK_API_VERSION_1_4);
// VkInstance createInstance(VkApplicationInfo const &appInfo);
// VkInstance
// createInstance(VkApplicationInfo const &appInfo,
//                VkDebugUtilsMessengerCreateInfoEXT const &debugCreateInfo);
// bool isDeviceExtsSupported(VkPhysicalDevice device);
// vec<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice
// device); SwapchainSupport getSwapchainSupport(VkPhysicalDevice device,
//                                      VkSurfaceKHR surface);
// vec<VkPhysicalDevice> getPhysicalDevices(VkInstance instance);
// vec<VkImage> getSwapchainImages(VkDevice device, VkSwapchainKHR swapchain);
// VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);
// VkShaderModule createShaderModule(VkDevice device, vec<i8> const &code);

// } // namespace Terreate::API
