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

#include <Terreate.hpp>
#include <api.hpp>
#include <context.hpp>
#include <debugger.hpp>
#include <instance.hpp>
#include <type.hpp>

using namespace Terreate::Type;

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

class TestDebugger : public Terreate::Core::IDebugger {
public:
  bool verbose(str const &message, Terreate::Core::MessageType const type,
               vec<Terreate::Core::DebugObject> const &object) override {
    std::cout << message << std::endl;
    return false;
  }
  bool info(str const &message, Terreate::Core::MessageType const type,
            vec<Terreate::Core::DebugObject> const &object) override {
    std::cout << message << std::endl;
    return false;
  }
  bool warning(str const &message, Terreate::Core::MessageType const type,
               vec<Terreate::Core::DebugObject> const &object) override {
    std::cout << message << std::endl;
    return false;
  }
  bool error(str const &message, Terreate::Core::MessageType const type,
             vec<Terreate::Core::DebugObject> const &object) override {
    std::cout << message << std::endl;
    return false;
  }
};

class VulkanTriangle {
private:
  GLFWwindow *mWindow;
  shared<Terreate::Core::VulkanInstance> mVulkanInstance;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  std::multimap<int, VkPhysicalDevice> mDevices;
  VkDevice mDevice;
  VkQueue mGraphicsQueue;
  VkQueue mPresentQueue;
  VkSurfaceKHR mSurface;
  VkSwapchainKHR mSwapchain;
  SwapchainProperty mSwapchainProperty;
  vec<VkImage> mSwapchainImages;
  vec<VkImageView> mSwapchainImageViews;
  vec<VkFramebuffer> mSwapchainFramebuffers;
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
  pickSurfaceFormat(vec<VkSurfaceFormatKHR> const &availableFormats);
  VkPresentModeKHR
  pickPresentMode(vec<VkPresentModeKHR> const &availablePresentModes);
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
