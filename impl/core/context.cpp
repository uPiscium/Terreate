#include "../../include/common/exception.hpp"
#include "../../include/core/context.hpp"
#include "../../include/vulkan/vk.hpp"

namespace Terreate::Core {
void Context::loadEXTfunctions() {
#ifdef TERREATE_DEBUG_BUILD
  Vulkan::trCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkCreateDebugUtilsMessengerEXT");
  Vulkan::trDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkDestroyDebugUtilsMessengerEXT");

  if (!Vulkan::trCreateDebugUtilsMessengerEXT ||
      !Vulkan::trDestroyDebugUtilsMessengerEXT) {
    Type::str msg =
        "Failed to load debug messenger functions. Make sure the project "
        "is built with TERREATE_DEBUG_BUILD=ON.";
    throw Exception::DebugMessengerFunctionLoadFailure(msg);
  }
#endif
}

Type::vec<char const *> Context::getRequiredExts() {
  Type::u32 glfwExtensionCount = 0;
  char const **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  Type::vec<char const *> extensions(glfwExtensions,
                                     glfwExtensions + glfwExtensionCount);
#ifdef TERREATE_DEBUG_BUILD
  extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  return extensions;
}

bool Context::checkValidationLayerSupport() {
  Type::u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  Type::vec<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (char const *layerName : Vulkan::VALIDATION_LAYERS) {
    bool layerFound = false;

    for (auto const &layerProp : availableLayers) {
      if (std::strcmp(layerName, layerProp.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      Type::str msg =
          "Validation layer " + Type::str(layerName) + " not found.";
      throw Exception::ValidationLayerNotSupported(msg);
    }
  }

  return true;
}

VkDebugUtilsMessengerCreateInfoEXT Context::createDebugInfo() {
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = Core::debugCallbackWrapper;
  createInfo.pUserData = nullptr; // Optional
  createInfo.pNext = nullptr;     // Optional
  return createInfo;
}

VkApplicationInfo Context::createAppInfo(char const *appName,
                                         Type::Version appVersion) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName;
  appInfo.applicationVersion =
      VK_MAKE_VERSION(appVersion.major, appVersion.minor, appVersion.patch);
  appInfo.pEngineName = "Terreate";
  appInfo.engineVersion = VK_MAKE_VERSION(Type::TERREATE_ENGINE_VERSION.major,
                                          Type::TERREATE_ENGINE_VERSION.minor,
                                          Type::TERREATE_ENGINE_VERSION.patch);
  appInfo.apiVersion = VK_API_VERSION_1_4;
  return appInfo;
}

Context::Context(Type::str const &appName, Type::Version appVersion) {
  if (!glfwInit()) {
    Type::str msg = "Failed to initialize GLFW. Make sure it is installed and "
                    "configured correctly.";
    throw Exception::GLFWInitializeFailure(msg);
  }

  auto appInfo = this->createAppInfo(appName.c_str(), appVersion);
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
  createInfo.pNext = nullptr;

#ifdef TERREATE_DEBUG_BUILD
  auto debuggerInfo = this->createDebugInfo();
  createInfo.enabledLayerCount =
      static_cast<Type::u32>(Vulkan::VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = Vulkan::VALIDATION_LAYERS.data();
  createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debuggerInfo;
#endif

  auto exts = this->getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<Type::u32>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

  if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
    Type::str msg =
        "Failed to create Vulkan instance. Make sure the Vulkan SDK is "
        "installed and configured correctly.";
    throw Exception::InstanceCreationFailure(msg);
  }

  this->loadEXTfunctions();
}

void Context::attachDebugger(Core::IDebugger *debugger) {
#ifdef TERREATE_DEBUG_BUILD
  if (!debugger)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = this->createDebugInfo();
  createInfo.pUserData = debugger;

  if (Vulkan::trCreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr,
                                             &mDebugMessenger) != VK_SUCCESS) {
    Type::str msg =
        "Failed to create debug messenger. Make sure the Vulkan SDK is "
        "installed and configured correctly.";
    throw Exception::DebugMessengerCreationFailure(msg);
  }
#endif
}

Vulkan::VkObjectRef<Vulkan::IWindow>
Context::createWindow(Type::str const &title, Type::pair<Type::i32> const &size,
                      Vulkan::WindowSettings const &settings) {
  Vulkan::VkObject<Vulkan::IWindow> window =
      makeVkObject<Vulkan::Window>(mInstance, title, size, settings);
  mWindows.emplace_back(std::move(window));
  return mWindows.back().ref();
}

Vulkan::VkObjectRef<Vulkan::ISurface>
Context::createSurface(Vulkan::VkObjectRef<Vulkan::IWindow> window) {
  if (!mInstance) {
    throw Exception::NullReferenceException(
        "Instance is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::ISurface> surface =
      makeVkObject<Vulkan::Surface>(mInstance, window);
  mSurfaces.emplace_back(std::move(surface));

  if (!mDevice) {
    mDevice = makeVkObject<Vulkan::Device>(mInstance, mSurfaces.back().ref());
  }

  return mSurfaces.back().ref();
}

Vulkan::VkObjectRef<Vulkan::ISwapchain>
Context::createSwapchain(Vulkan::VkObjectRef<Vulkan::IWindow> window,
                         Vulkan::VkObjectRef<Vulkan::ISurface> surface) {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::ISwapchain> swapchain =
      makeVkObject<Vulkan::Swapchain>(
          mDevice.ref(), window->getProperties().framebufferSize, surface);
  mSwapchains.emplace_back(std::move(swapchain));
  return mSwapchains.back().ref();
}

Vulkan::VkObjectRef<Vulkan::IGraphicQueue> Context::createGraphicQueue() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::IGraphicQueue> graphicQueue =
      makeVkObject<Vulkan::GraphicQueue>(mDevice.ref());
  mGraphicQueues.emplace_back(std::move(graphicQueue));
  return mGraphicQueues.back().ref();
}

Vulkan::VkObjectRef<Vulkan::IPresentQueue> Context::createPresentQueue() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::IPresentQueue> presentQueue =
      makeVkObject<Vulkan::PresentQueue>(mDevice.ref());
  mPresentQueues.emplace_back(std::move(presentQueue));
  return mPresentQueues.back().ref();
}

Vulkan::VkObjectRef<Vulkan::IRenderPass>
Context::createRenderPass(Vulkan::VkObjectRef<Vulkan::ISwapchain> swapchain) {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::IRenderPass> renderPass =
      makeVkObject<Vulkan::RenderPass>(mDevice.ref(), swapchain);
  mRenderPasses.emplace_back(std::move(renderPass));
  return mRenderPasses.back().ref();
}

Vulkan::VkObjectRef<Vulkan::IPipeline>
Context::createPipeline(Vulkan::VkObjectRef<Vulkan::IRenderPass> renderPass) {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::IPipeline> pipeline =
      makeVkObject<Vulkan::Pipeline>(mDevice.ref(), renderPass);
  mPipelines.emplace_back(std::move(pipeline));
  return mPipelines.back().ref();
}

Vulkan::VkObjectRef<Vulkan::IFramebuffer>
Context::createFramebuffer(Vulkan::VkObjectRef<Vulkan::IRenderPass> renderPass,
                           Vulkan::VkObjectRef<Vulkan::ISwapchain> swapchain) {
  Vulkan::VkObject<Vulkan::IFramebuffer> framebuffer =
      makeVkObject<Vulkan::Framebuffer>(mDevice.ref(), renderPass, swapchain);
  mFramebuffers.emplace_back(std::move(framebuffer));
  return mFramebuffers.back().ref();
}

Vulkan::VkObjectRef<Vulkan::ICommandPool> Context::createCommandPool() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::ICommandPool> commandPool =
      makeVkObject<Vulkan::CommandPool>(mDevice.ref());
  mCommandPools.emplace_back(std::move(commandPool));
  return mCommandPools.back().ref();
}

Vulkan::VkObjectRef<Vulkan::ISemaphore> Context::createSemaphore() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::ISemaphore> semaphore =
      makeVkObject<Vulkan::Semaphore>(mDevice.ref());
  mSemaphores.emplace_back(std::move(semaphore));
  return mSemaphores.back().ref();
}

Vulkan::VkObjectRef<Vulkan::IFence> Context::createFence() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  Vulkan::VkObject<Vulkan::IFence> fence =
      makeVkObject<Vulkan::Fence>(mDevice.ref());
  mFences.emplace_back(std::move(fence));
  return mFences.back().ref();
}

void Context::dispose() {
  vkDeviceWaitIdle(*mDevice);

  for (auto &fence : mFences) {
    fence.dispose();
  }
  mFences.clear();

  for (auto &semaphore : mSemaphores) {
    semaphore.dispose();
  }
  mSemaphores.clear();

  for (auto &commandPool : mCommandPools) {
    commandPool.dispose();
  }
  mCommandPools.clear();

  for (auto &framebuffer : mFramebuffers) {
    framebuffer.dispose();
  }
  mFramebuffers.clear();

  for (auto &pipeline : mPipelines) {
    pipeline.dispose();
  }
  mPipelines.clear();

  for (auto &renderPass : mRenderPasses) {
    renderPass.dispose();
  }
  mRenderPasses.clear();

  for (auto &presentQueue : mPresentQueues) {
    presentQueue.dispose();
  }
  mPresentQueues.clear();

  for (auto &graphicQueue : mGraphicQueues) {
    graphicQueue.dispose();
  }
  mGraphicQueues.clear();

  for (auto &swapchain : mSwapchains) {
    swapchain.dispose();
  }
  mSwapchains.clear();

  for (auto &surface : mSurfaces) {
    surface.dispose();
  }
  mSurfaces.clear();

  for (auto &window : mWindows) {
    window.dispose();
  }
  mWindows.clear();

  if (mDevice) {
    mDevice.dispose();
  }

  if (mDebugMessenger != VK_NULL_HANDLE) {
    Vulkan::trDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger,
                                            nullptr);
    mDebugMessenger = VK_NULL_HANDLE;
  }

  if (mInstance != VK_NULL_HANDLE) {
    vkDestroyInstance(mInstance, nullptr);
    mInstance = VK_NULL_HANDLE;
  }

  glfwTerminate();
}

} // namespace Terreate::Core
