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
  Type::vec<VkObject<Surface>> mSurfaces;
  Type::vec<VkObject<Swapchain>> mSwapchains;
  Type::vec<VkObject<GraphicQueue>> mGraphicQueues;
  Type::vec<VkObject<PresentQueue>> mPresentQueues;
  Type::vec<VkObject<RenderPass>> mRenderPasses;
  Type::vec<VkObject<Pipeline>> mPipelines;
  Type::vec<VkObject<Framebuffer>> mFramebuffers;
  Type::vec<VkObject<ICommandPool>> mCommandPools;
  Type::vec<VkObject<Semaphore>> mSemaphores;
  Type::vec<VkObject<Fence>> mFences;

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
  VkObjectRef<Surface> createSurface(VkObjectRef<Window> window);
  VkObjectRef<Swapchain> createSwapchain(VkObjectRef<Window> window,
                                         VkObjectRef<Surface> surface);
  VkObjectRef<GraphicQueue> createGraphicQueue();
  VkObjectRef<PresentQueue> createPresentQueue();
  VkObjectRef<RenderPass> createRenderPass(VkObjectRef<Swapchain> swapchain);
  VkObjectRef<Pipeline> createPipeline(VkObjectRef<Swapchain> swapchain,
                                       VkObjectRef<RenderPass> renderPass);
  VkObjectRef<Framebuffer> createFramebuffer(VkObjectRef<Pipeline> pipeline);
  VkObjectRef<ICommandPool> createCommandPool(VkObjectRef<Pipeline> pipeline);
  VkObjectRef<Semaphore> createSemaphore();
  VkObjectRef<Fence> createFence();

  void dispose();

  Context &operator=(Context &&other) noexcept;
};
} // namespace Terreate::Core
