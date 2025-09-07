#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

typedef std::string str;
typedef uint32_t u32;

template <typename T> using vec = std::vector<T>;

static constexpr char const *ENGINE_NAME = "Terreate";
static constexpr int WINDOW_WIDTH = 800;
static constexpr int WINDOW_HEIGHT = 600;
static constexpr u32 ENGINE_VERSION = VK_MAKE_VERSION(0, 1, 0);

static constexpr char const *VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"};

class VulkanTutorial {
private:
  bool mDebugMode = false;

  VkInstance mInstance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;

  SDL_Window *mWindow = nullptr;

private:
  void initWindow();
  bool checkValidationLayerSupport();
  vec<char const *> getRequiredExtensions();
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void createInstance(str const &appName, u32 const &appVersion);
  VkResult createDebugUtilsMessengerEXT(
      VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
      VkAllocationCallbacks const *pAllocator);
  VkResult
  destroyDebugUtilsMessengerEXT(VkAllocationCallbacks const *pAllocator);
  void setupDebugMessenger();
  void initVulkan();
  bool pollEvents();
  void mainLoop();
  void cleanup();

public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                void *pUserData);

public:
  VulkanTutorial(bool debug = false) : mDebugMode(debug) {}
  void run();
};

int main();
