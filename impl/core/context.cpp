#include "../../include/common/exception.hpp"
#include "../../include/core/context.hpp"
#include "../../include/core/swapchain.hpp"
#include "../../include/core/vk.hpp"

namespace Terreate::Core {
void Context::loadEXTfunctions() {
#ifdef TERREATE_DEBUG_BUILD
  trCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkCreateDebugUtilsMessengerEXT");
  trDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkDestroyDebugUtilsMessengerEXT");

  if (!trCreateDebugUtilsMessengerEXT || !trDestroyDebugUtilsMessengerEXT) {
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

  for (char const *layerName : VALIDATION_LAYERS) {
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
  createInfo.pfnUserCallback = debugCallbackWrapper;
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
      static_cast<Type::u32>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
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

void Context::attachDebugger(IDebugger *debugger) {
#ifdef TERREATE_DEBUG_BUILD
  if (!debugger)
    return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = this->createDebugInfo();
  createInfo.pUserData = debugger;

  if (trCreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr,
                                     &mDebugMessenger) != VK_SUCCESS) {
    Type::str msg =
        "Failed to create debug messenger. Make sure the Vulkan SDK is "
        "installed and configured correctly.";
    throw Exception::DebugMessengerCreationFailure(msg);
  }
#endif
}

VkObjectRef<Window> Context::createWindow(Type::str const &title,
                                          Type::pair<Type::i32> const &size,
                                          WindowSettings const &settings) {
  auto window = makeVkObject<Window>(mInstance, title, size, settings);
  mWindows.emplace_back(std::move(window));
  auto ref = mWindows.back().ref();
  if (!mDevice) {
    mDevice = makeVkObject<Device>(mInstance, ref->getSurface());
  }

  auto swapchain = makeVkObject<Swapchain>(
      mDevice.get(), ref->properties.framebufferSize, ref->getSurface());
  ref->attachSwapchain(std::move(swapchain));

  return ref;
}

VkObjectRef<GraphicQueue> Context::createGraphicQueue() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  auto graphicQueue = makeVkObject<GraphicQueue>(mDevice.get());
  mGraphicQueues.emplace_back(std::move(graphicQueue));
  return mGraphicQueues.back().ref();
}

VkObjectRef<PresentQueue> Context::createPresentQueue() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  auto presentQueue = makeVkObject<PresentQueue>(mDevice.get());
  mPresentQueues.emplace_back(std::move(presentQueue));
  return mPresentQueues.back().ref();
}

VkObjectRef<Pipeline> Context::createPipeline(VkObjectRef<Window> window) {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  auto pipeline = makeVkObject<Pipeline>(mDevice.get(), window->getSwapchain());
  mPipelines.emplace_back(std::move(pipeline));
  return mPipelines.back().ref();
}

VkObjectRef<Framebuffer>
Context::createFramebuffer(VkObjectRef<Pipeline> pipeline) {
  auto framebuffer = makeVkObject<Framebuffer>(pipeline);
  mFramebuffers.emplace_back(std::move(framebuffer));
  return mFramebuffers.back().ref();
}

VkObjectRef<CommandPool>
Context::createCommandPool(VkObjectRef<Pipeline> pipeline) {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  auto commandPool = makeVkObject<CommandPool>(pipeline);
  mCommandPools.emplace_back(std::move(commandPool));
  return mCommandPools.back().ref();
}

VkObjectRef<Semaphore> Context::createSemaphore() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  auto semaphore = makeVkObject<Semaphore>(mDevice.get());
  mSemaphores.emplace_back(std::move(semaphore));
  return mSemaphores.back().ref();
}

VkObjectRef<Fence> Context::createFence() {
  if (!mDevice) {
    throw Exception::NullReferenceException(
        "Device is not initialized. Please create a window first.");
  }

  auto fence = makeVkObject<Fence>(mDevice.get());
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

  for (auto &presentQueue : mPresentQueues) {
    presentQueue.dispose();
  }
  mPresentQueues.clear();

  for (auto &graphicQueue : mGraphicQueues) {
    graphicQueue.dispose();
  }
  mGraphicQueues.clear();

  for (auto &window : mWindows) {
    window.dispose();
  }
  mWindows.clear();

  if (mDevice) {
    mDevice.dispose();
  }

  if (mDebugMessenger != VK_NULL_HANDLE) {
    trDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
    mDebugMessenger = VK_NULL_HANDLE;
  }

  if (mInstance != VK_NULL_HANDLE) {
    vkDestroyInstance(mInstance, nullptr);
    mInstance = VK_NULL_HANDLE;
  }

  glfwTerminate();
}

} // namespace Terreate::Core
