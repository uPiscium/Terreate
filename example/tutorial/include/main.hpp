#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

typedef uint32_t u32;
typedef std::string str;

template <typename T> using vec = std::vector<T>;

#define TERREATE_ENGINE_VERSION_MAJOR 0
#define TERREATE_ENGINE_VERSION_MINOR 1
#define TERREATE_ENGINE_VERSION_PATCH 0
#define TERREATE_ENGINE_VERSION                                                \
  VK_MAKE_VERSION(TERREATE_ENGINE_VERSION_MAJOR,                               \
                  TERREATE_ENGINE_VERSION_MINOR,                               \
                  TERREATE_ENGINE_VERSION_PATCH)

static vec<char const *> const ENABLED_INSTANCE_LAYERS = {
    "VK_LAYER_KHRONOS_validation"};

struct QueueFamilyIndices {
  u32 graphicsFamily = UINT32_MAX;
  // u32 presentFamily = UINT32_MAX;

  bool isComplete() const {
    return graphicsFamily != UINT32_MAX; //&& presentFamily != UINT32_MAX;
  }
};

class VulkanTriangle {
private:
  SDL_Window *mWindow = nullptr;
  vk::UniqueInstance mInstance;
  vk::UniqueDebugUtilsMessengerEXT mDebugMessenger;
  vk::PhysicalDevice mPhysicalDevice;
  vk::UniqueDevice mDevice;

#ifdef TERREATE_DEBUG_BUILD
private:
  bool isLayerSupported(vec<char const *> const &layers) const;
  void setupDebugMessenger();

public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                vk::DebugUtilsMessageTypeFlagsEXT messageType,
                vk::DebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                void *pUserData);
  static void populateDebugMessengerCreateInfo(
      vk::DebugUtilsMessengerCreateInfoEXT &info,
      vk::PFN_DebugUtilsMessengerCallbackEXT callback);
#endif

private:
  void initSDL() const;
  SDL_Window *initWindow(u32 const &width, u32 const &height,
                         str const &title) const;
  vec<char const *> getRequiredExtensions() const;
  vk::UniqueInstance createInstance(str const &appName,
                                    u32 const &appVersion) const;
  QueueFamilyIndices findQueueFamily(vk::PhysicalDevice const &device) const;
  int rateDevice(vk::PhysicalDevice const &device) const;
  vk::PhysicalDevice
  pickPhysicalDevice(vk::UniqueInstance const &instance) const;
  void initVulkan();
  void mainLoop();
  void cleanup();

public:
  void run();
};

int main();
