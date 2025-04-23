#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <optional>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <api.hpp>
#include <type.hpp>

int const WIDTH = 800;
int const HEIGHT = 600;
int const DISCRETE_GPU_SCORE = 1000;
int const INTEGRATED_GPU_SCORE = 100;
int const VIRTUAL_GPU_SCORE = 10;
std::vector<char const *> const DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
};

struct SwapchainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct SwapchainProperty {
  VkFormat imageFormat;
  VkExtent2D extent;
};

using namespace Terreate::API;

class TestDebugger : public Debugger {
public:
  bool verbose(str const &message, Debugger::MessageType const type,
               vec<DebugObject> const &object) override {
    std::cout << message << std::endl;
    return true;
  }
  bool info(str const &message, Debugger::MessageType const type,
            vec<DebugObject> const &object) override {
    std::cout << message << std::endl;
    return true;
  }
  bool warning(str const &message, Debugger::MessageType const type,
               vec<DebugObject> const &object) override {
    std::cout << message << std::endl;
    return true;
  }
  bool error(str const &message, Debugger::MessageType const type,
             vec<DebugObject> const &object) override {
    std::cout << message << std::endl;
    return true;
  }
};

class VulkanTriangle {
private:
  TestDebugger *mDebugger;
  GLFWwindow *mWindow;
  VkInstance mInstance;
  VkDebugUtilsMessengerEXT mDebugMessenger;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  std::multimap<int, VkPhysicalDevice> mDevices;
  VkDevice mDevice;
  VkQueue mGraphicsQueue;
  VkQueue mPresentQueue;
  VkSurfaceKHR mSurface;
  VkSwapchainKHR mSwapchain;
  SwapchainProperty mSwapchainProperty;
  std::vector<VkImage> mSwapchainImages;
  std::vector<VkImageView> mSwapchainImageViews;
  std::vector<VkFramebuffer> mSwapchainFramebuffers;
  VkRenderPass mRenderPass;
  VkPipelineLayout mPipelineLayout;
  VkPipeline mGraphicsPipeline;
  VkCommandPool mCommandPool;
  VkCommandBuffer mCommandBuffer;
  VkSemaphore mImageAvailableSemaphore;
  VkSemaphore mRenderFinishedSemaphore;
  VkFence mInFlightFence;

private:
  static std::vector<char> readFile(std::string const &filename);

public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                void *pUserData);

private:
  void initWindow();
  void loadDebugUtilsMessengerEXT();

  bool checkValidationLayerSupport();
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void setupDebugMessenger();
  VkApplicationInfo createAppInfo();
  std::vector<char const *> getRequiredExtensions();
  void createInstance();
  bool isCompleteQueueFamily(QueueFamilyIndices const &queueFamily);

  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  int rateDeviceSuitability(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSurface();
  VkSurfaceFormatKHR
  pickSurfaceFormat(std::vector<VkSurfaceFormatKHR> const &availableFormats);
  VkPresentModeKHR
  pickPresentMode(std::vector<VkPresentModeKHR> const &availablePresentModes);
  VkExtent2D pickExtent(VkSurfaceCapabilitiesKHR const &capabilities);
  void createSwapchain();
  void createImageViews();
  VkShaderModule createShaderModule(std::vector<char> const &code);
  void createRenderPass();
  void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandPool();
  void createCommandBuffer();
  void recordCommandBuffer(VkCommandBuffer cmdBuffer, size_t imgIndex);
  void createSyncObjects();
  void drawFrame();
  void initVulkan();
  void mainLoop();
  void cleanup();

public:
  VulkanTriangle();
  ~VulkanTriangle();

  void run();
};

int main();
