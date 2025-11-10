#pragma once

#include <Terreate/common.hpp>
#include <Terreate/sdl.hpp>
#include <Terreate/vulkan.hpp>

using namespace Terreate;

class App {
private:
  bool mDebugMode = true;

  shared<SDL::Registry> mSDLRegistry = nullptr;
  // shared<SDL::EventHandler> mSDLEventHandler = nullptr;
  shared<Vulkan::Instance> mInstance = nullptr;
  shared<Vulkan::DefaultDebugger> mDebugger = nullptr;
  shared<SDL::Window> mWindow = nullptr;
  shared<Vulkan::Device> mDevice = nullptr;
  shared<Vulkan::Queue> mQueue = nullptr;

  shared<Vulkan::Swapchain> mSwapchain = nullptr;
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
