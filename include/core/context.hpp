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
  Vulkan::VkObject<Vulkan::IDevice> mDevice = nullptr;

  Type::vec<Vulkan::VkObject<Vulkan::IWindow>> mWindows;
  Type::vec<Vulkan::VkObject<Vulkan::ISurface>> mSurfaces;
  Type::vec<Vulkan::VkObject<Vulkan::ISwapchain>> mSwapchains;
  Type::vec<Vulkan::VkObject<Vulkan::IGraphicQueue>> mGraphicQueues;
  Type::vec<Vulkan::VkObject<Vulkan::IPresentQueue>> mPresentQueues;
  Type::vec<Vulkan::VkObject<Vulkan::IRenderPass>> mRenderPasses;
  Type::vec<Vulkan::VkObject<Vulkan::IPipeline>> mPipelines;
  Type::vec<Vulkan::VkObject<Vulkan::IFramebuffer>> mFramebuffers;
  Type::vec<Vulkan::VkObject<Vulkan::ICommandPool>> mCommandPools;
  Type::vec<Vulkan::VkObject<Vulkan::ISemaphore>> mSemaphores;
  Type::vec<Vulkan::VkObject<Vulkan::IFence>> mFences;

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
  Vulkan::VkObjectRef<Vulkan::IDevice> getDevice() const {
    return mDevice.ref();
  }

  void attachDebugger(IDebugger *debugger);

  Vulkan::VkObjectRef<Vulkan::IWindow> createWindow(
      Type::str const &title, Type::pair<Type::i32> const &size,
      Vulkan::WindowSettings const &settings = Vulkan::WindowSettings());
  Vulkan::VkObjectRef<Vulkan::ISurface>
  createSurface(Vulkan::VkObjectRef<Vulkan::IWindow> window);
  Vulkan::VkObjectRef<Vulkan::ISwapchain>
  createSwapchain(Vulkan::VkObjectRef<Vulkan::IWindow> window,
                  Vulkan::VkObjectRef<Vulkan::ISurface> surface);
  Vulkan::VkObjectRef<Vulkan::IGraphicQueue> createGraphicQueue();
  Vulkan::VkObjectRef<Vulkan::IPresentQueue> createPresentQueue();
  Vulkan::VkObjectRef<Vulkan::IRenderPass>
  createRenderPass(Vulkan::VkObjectRef<Vulkan::ISwapchain> swapchain);
  Vulkan::VkObjectRef<Vulkan::IPipeline>
  createPipeline(Vulkan::VkObjectRef<Vulkan::IRenderPass> renderPass);
  Vulkan::VkObjectRef<Vulkan::IFramebuffer>
  createFramebuffer(Vulkan::VkObjectRef<Vulkan::IRenderPass> renderPass,
                    Vulkan::VkObjectRef<Vulkan::ISwapchain> swapchain);
  Vulkan::VkObjectRef<Vulkan::ICommandPool> createCommandPool();
  Vulkan::VkObjectRef<Vulkan::ISemaphore> createSemaphore();
  Vulkan::VkObjectRef<Vulkan::IFence> createFence();

  void dispose();

  Context &operator=(Context &&other) noexcept;
};
} // namespace Terreate::Core
