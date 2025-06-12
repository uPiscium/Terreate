#pragma once
#include "command.hpp"
#include "debugger.hpp"
#include "device.hpp"
#include "framebuffer.hpp"
#include "pipeline.hpp"
#include "queue.hpp"
#include "renderpass.hpp"
#include "surface.hpp"
#include "sync.hpp"
#include "vkobj.hpp"
#include "window.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class Context {
private:
  PROHIBIT_COPY_AND_ASSIGN(Context);

private:
  VkInstance mInstance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
  VkObject<Device> mDevice = nullptr;

  Type::vec<VkObject<Window>> mWindows;
  Type::vec<VkObject<ISurface>> mSurfaces;
  Type::vec<VkObject<ISwapchain>> mSwapchains;
  Type::vec<VkObject<IGraphicQueue>> mGraphicQueues;
  Type::vec<VkObject<IPresentQueue>> mPresentQueues;
  Type::vec<VkObject<IRenderPass>> mRenderPasses;
  Type::vec<VkObject<IPipeline>> mPipelines;
  Type::vec<VkObject<IFramebuffer>> mFramebuffers;
  Type::vec<VkObject<ICommandPool>> mCommandPools;
  Type::vec<VkObject<ISemaphore>> mSemaphores;
  Type::vec<VkObject<IFence>> mFences;

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
  VkObjectRef<Device> getDevice() const { return mDevice.ref(); }

  void attachDebugger(IDebugger *debugger);

  VkObjectRef<Window>
  createWindow(Type::str const &title, Type::pair<Type::i32> const &size,
               WindowSettings const &settings = WindowSettings());
  VkObjectRef<ISurface> createSurface(VkObjectRef<Window> window);
  VkObjectRef<ISwapchain> createSwapchain(VkObjectRef<Window> window,
                                          VkObjectRef<ISurface> surface);
  VkObjectRef<IGraphicQueue> createGraphicQueue();
  VkObjectRef<IPresentQueue> createPresentQueue();
  VkObjectRef<IRenderPass> createRenderPass(VkObjectRef<ISwapchain> swapchain);
  VkObjectRef<IPipeline> createPipeline(VkObjectRef<IRenderPass> renderPass);
  VkObjectRef<IFramebuffer>
  createFramebuffer(VkObjectRef<IRenderPass> renderPass,
                    VkObjectRef<ISwapchain> swapchain);
  VkObjectRef<ICommandPool> createCommandPool();
  VkObjectRef<ISemaphore> createSemaphore();
  VkObjectRef<IFence> createFence();

  void dispose();

  Context &operator=(Context &&other) noexcept;
};
} // namespace Terreate::Core
