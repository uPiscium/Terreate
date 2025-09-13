#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef std::string str;
typedef int32_t i32;
typedef uint32_t u32;

template <typename T> using vec = std::vector<T>;
template <typename K, typename V> using umap = std::unordered_map<K, V>;
template <typename T> using uset = std::unordered_set<T>;

static constexpr char const *ENGINE_NAME = "Terreate";
static constexpr int WINDOW_WIDTH = 800;
static constexpr int WINDOW_HEIGHT = 600;
static constexpr u32 ENGINE_VERSION = VK_MAKE_VERSION(0, 1, 0);

static constexpr char const *VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"};
static constexpr char const *DEVICE_EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct QueueFamilyIndices {
  i32 graphicsFamily = -1;
  i32 presentFamily = -1;

  bool isComplete() const { return graphicsFamily * presentFamily >= 0; }
  operator uset<i32>() const;
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  vec<VkSurfaceFormatKHR> formats;
  vec<VkPresentModeKHR> presentModes;
};

class VulkanTutorial {
private:
  bool mDebugMode = false;

  VkInstance mInstance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;
  VkQueue mGraphicsQueue = VK_NULL_HANDLE;
  VkQueue mPresentQueue = VK_NULL_HANDLE;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;
  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  vec<VkImage> mSwapchainImages;
  VkFormat mSwapchainImageFormat;
  VkExtent2D mSwapchainExtent;
  vec<VkImageView> mSwapchainImageViews;
  VkRenderPass mRenderPass = VK_NULL_HANDLE;
  VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;
  vec<VkFramebuffer> mSwapchainFramebuffers;
  VkCommandPool mCommandPool = VK_NULL_HANDLE;

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
  void createSurface();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  int rateDevice(VkPhysicalDevice device);
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createQueue();
  VkSurfaceFormatKHR
  chooseSwapSurfaceFormat(vec<VkSurfaceFormatKHR> const &availableFormats);
  VkPresentModeKHR
  chooseSwapPresentMode(vec<VkPresentModeKHR> const &availablePresentModes);
  VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR const &capabilities);
  void createSwapchain();
  void createImageViews();
  VkShaderModule createShaderModule(vec<char> const &code);
  void createRenderPass();
  void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandPool();
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
  static vec<char> readFile(str const &filename);

public:
  VulkanTutorial(bool debug = false) : mDebugMode(debug) {}
  void run();
};

int main();
