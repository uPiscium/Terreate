#pragma once

#include <Terreate/common.hpp>
#include <Terreate/sdl.hpp>
#include <Terreate/vulkan.hpp>

using namespace Terreate;

struct QueueFamilyIndices {
  i32 graphicsFamily = -1;
  i32 presentFamily = -1;

  bool isComplete() const { return graphicsFamily * presentFamily >= 0; }
  operator uset<i32>() const {
    if (this->isComplete()) {
      return uset<i32>{graphicsFamily, presentFamily};
    } else {
      return uset<i32>{};
    }
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  vec<VkSurfaceFormatKHR> formats;
  vec<VkPresentModeKHR> presentModes;
};

class App {
private:
  bool mDebugMode = true;

  // SDL_Window *mWindow;
  shared<SDL::Registry> mSDLRegistry = nullptr;
  // shared<SDL::EventHandler> mSDLEventHandler = nullptr;
  shared<Vulkan::Instance> mInstance = nullptr;
  shared<Vulkan::DefaultDebugger> mDebugger = nullptr;
  shared<SDL::Window> mWindow = nullptr;
  // VkSurfaceKHR mSurface = VK_NULL_HANDLE;

  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;

  VkQueue mGraphicsQueue = VK_NULL_HANDLE;
  VkQueue mPresentQueue = VK_NULL_HANDLE;

  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  vec<VkImage> mSwapchainImages;
  VkFormat mSwapchainImageFormat;
  VkExtent2D mSwapchainExtent;
  vec<VkImageView> mSwapchainImageViews;

  VkRenderPass mRenderPass = VK_NULL_HANDLE;
  VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
  VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;
  vec<VkFramebuffer> mSwapchainFramebuffers;

  VkCommandPool mCommandPool = VK_NULL_HANDLE;
  vec<VkCommandBuffer> mCommandBuffers = {};

  vec<VkSemaphore> mImageAvailableSemaphores = {};
  vec<VkSemaphore> mRenderFinishedSemaphores = {};
  vec<VkFence> mInFlightFences = {};

  u32 mCurrentFrame = 0;
  bool mFramebufferResized = false;

private:
  void initWindow(int const &width, int const &height, str const &title);
  // void createSurface();

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
  void createCommandPool();
  void createRenderPass();
  void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandBuffers();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);
  void createSyncObjects();

  void cleanupSwapchain();
  void recreateSwapchain();

  void initVulkan();
  bool pollEvents();
  void drawFrame();
  void mainLoop();
  void cleanup();

public:
  App(int const &width = 1500, int const &height = 750,
      str const &title = "Vulkan Tutorial", bool debugMode = true);
  ~App();

  void run();
};

int main();
