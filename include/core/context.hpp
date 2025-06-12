#pragma once
#include "debugger.hpp"

#include "../vulkan/command.hpp"
#include "../vulkan/device.hpp"
#include "../vulkan/framebuffer.hpp"
#include "../vulkan/pipeline.hpp"
#include "../vulkan/queue.hpp"
#include "../vulkan/renderpass.hpp"
#include "../vulkan/surface.hpp"
#include "../vulkan/sync.hpp"
#include "../vulkan/vkobj.hpp"
#include "../vulkan/window.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class Context {
private:
  PROHIBIT_COPY_AND_ASSIGN(Context);

private:
  VkInstance mInstance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
  Vulkan::VkObject<Vulkan::Device> mDevice = nullptr;

  Type::vec<Vulkan::VkObject<Vulkan::Window>> mWindows;
  Type::vec<Vulkan::VkObject<Vulkan::Surface>> mSurfaces;
  Type::vec<Vulkan::VkObject<Vulkan::Swapchain>> mSwapchains;
  Type::vec<Vulkan::VkObject<Vulkan::GraphicQueue>> mGraphicQueues;
  Type::vec<Vulkan::VkObject<Vulkan::PresentQueue>> mPresentQueues;
  Type::vec<Vulkan::VkObject<Vulkan::RenderPass>> mRenderPasses;
  Type::vec<Vulkan::VkObject<Vulkan::Pipeline>> mPipelines;
  Type::vec<Vulkan::VkObject<Vulkan::Framebuffer>> mFramebuffers;
  Type::vec<Vulkan::VkObject<Vulkan::CommandPool>> mCommandPools;
  Type::vec<Vulkan::VkObject<Vulkan::Semaphore>> mSemaphores;
  Type::vec<Vulkan::VkObject<Vulkan::Fence>> mFences;

private:
  void loadEXTfunctions();
  Type::vec<char const *> getRequiredExts();
  bool checkValidationLayerSupport();
  VkDebugUtilsMessengerCreateInfoEXT createDebugInfo();
  VkApplicationInfo createAppInfo(char const *appName,
                                  Type::Version appVersion);

public:
  Context() = default;
  Context(Type::str const &appName, Type::Version appVersion);
  Context(Context &&other) noexcept;
  ~Context() { this->dispose(); }

  VkInstance getInstance() { return mInstance; }
  Vulkan::VkObjectRef<Vulkan::Device> getDevice() const {
    return mDevice.ref();
  }

  void attachDebugger(IDebugger *debugger);

  Vulkan::VkObjectRef<Vulkan::Window> createWindow(
      Type::str const &title, Type::pair<Type::i32> const &size,
      Vulkan::WindowSettings const &settings = Vulkan::WindowSettings());
  Vulkan::VkObjectRef<Vulkan::Surface>
  createSurface(Vulkan::VkObjectRef<Vulkan::Window> window);
  Vulkan::VkObjectRef<Vulkan::Swapchain>
  createSwapchain(Vulkan::VkObjectRef<Vulkan::Window> window,
                  Vulkan::VkObjectRef<Vulkan::Surface> surface);
  Vulkan::VkObjectRef<Vulkan::GraphicQueue> createGraphicQueue();
  Vulkan::VkObjectRef<Vulkan::PresentQueue> createPresentQueue();
  Vulkan::VkObjectRef<Vulkan::RenderPass>
  createRenderPass(Vulkan::VkObjectRef<Vulkan::Swapchain> swapchain);
  Vulkan::VkObjectRef<Vulkan::Pipeline>
  createPipeline(Vulkan::VkObjectRef<Vulkan::RenderPass> renderPass);
  Vulkan::VkObjectRef<Vulkan::Framebuffer>
  createFramebuffer(Vulkan::VkObjectRef<Vulkan::RenderPass> renderPass,
                    Vulkan::VkObjectRef<Vulkan::Swapchain> swapchain);
  Vulkan::VkObjectRef<Vulkan::CommandPool> createCommandPool();
  Vulkan::VkObjectRef<Vulkan::Semaphore> createSemaphore();
  Vulkan::VkObjectRef<Vulkan::Fence> createFence();

  void dispose();

  Context &operator=(Context &&other) noexcept;
};
} // namespace Terreate::Core
