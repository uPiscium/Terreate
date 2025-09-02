#include "../include/main.hpp"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

#ifdef TERREATE_DEBUG_BUILD
bool VulkanTriangle::isLayerSupported(vec<char const *> const &layers) const {
  if (layers.empty()) {
    return true; // No layers to check, considered supported
  }

  u32 layerCount = 0;
  vk::Result result =
      vk::enumerateInstanceLayerProperties(&layerCount, nullptr);
  if (result != vk::Result::eSuccess) {
    std::cerr << "Failed to get instance layer count: " << vk::to_string(result)
              << std::endl;
    return false;
  }

  if (layerCount == 0) {
    std::cerr << "No Vulkan layers available." << std::endl;
    return false;
  }

  vec<vk::LayerProperties> availableLayers(layerCount);
  result =
      vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  if (result != vk::Result::eSuccess) {
    std::cerr << "Failed to enumerate instance layers: "
              << vk::to_string(result) << std::endl;
    return false;
  }

  for (auto const &layerName : layers) {
    bool found = false;
    for (auto const &layer : availableLayers) {
      if (std::strcmp(layer.layerName, layerName) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      std::cerr << "Layer not supported: " << layerName << std::endl;
      return false;
    }
  }

  return true;
}

void VulkanTriangle::populateDebugMessengerCreateInfo(
    vk::DebugUtilsMessengerCreateInfoEXT &info,
    vk::PFN_DebugUtilsMessengerCallbackEXT callback) {
  vk::DebugUtilsMessageSeverityFlagsEXT severity =
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
  vk::DebugUtilsMessageTypeFlagsEXT type =
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
  info = vk::DebugUtilsMessengerCreateInfoEXT{
      {}, severity, type, callback, nullptr};
}

void VulkanTriangle::setupDebugMessenger() {
  vk::DebugUtilsMessengerCreateInfoEXT createInfo;
  VulkanTriangle::populateDebugMessengerCreateInfo(
      createInfo, VulkanTriangle::debugCallback);

  try {
    mDebugMessenger = mInstance->createDebugUtilsMessengerEXTUnique(createInfo);
  } catch (vk::SystemError const &e) {
    std::cerr << "Failed to create debug messenger: " << e.what() << std::endl;
    throw std::runtime_error("Failed to create debug messenger");
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanTriangle::debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    vk::DebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
  switch (messageSeverity) {
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
    std::cout << "VERBOSE: ";
    break;
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
    std::cout << "INFO: ";
    break;
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
    std::cerr << "WARNING: ";
    break;
  case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
    std::cerr << "ERROR: ";
    break;
  default:
    std::cerr << "UNKNOWN: ";
    break;
  }
  std::cerr << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}
#endif

void VulkanTriangle::initSDL() const {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_CAMERA |
                SDL_INIT_SENSOR | SDL_INIT_AUDIO)) {
    std::cerr << "Failed to initialize SDL: " + str(SDL_GetError())
              << std::endl;
    return;
  }
}

SDL_Window *VulkanTriangle::initWindow(u32 const &width, u32 const &height,
                                       str const &title) const {
  this->initSDL();
  return SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_VULKAN);
}

vec<char const *> VulkanTriangle::getRequiredExtensions() const {
  u32 extCount = 0;
  auto exts = SDL_Vulkan_GetInstanceExtensions(&extCount);
  vec<char const *> extensions(exts, exts + extCount);

#ifdef TERREATE_DEBUG_BUILD
  // Add debug report extension if in debug mode
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  return extensions;
}

vk::UniqueInstance VulkanTriangle::createInstance(str const &appName,
                                                  u32 const &appVersion) const {
  vk::ApplicationInfo appInfo(appName.c_str(), appVersion, "Terreate",
                              TERREATE_ENGINE_VERSION, vk::ApiVersion14);

  vec<char const *> exts = this->getRequiredExtensions();

  // TODO: Support VK_ERROR_INCOMPATIBLE_DRIVER
  vk::InstanceCreateInfo createInfo;

#ifdef TERREATE_DEBUG_BUILD
  if (!this->isLayerSupported(ENABLED_INSTANCE_LAYERS)) {
    std::cerr << "Validation layer not supported." << std::endl;
    throw std::runtime_error("Validation layer not supported");
  }

  vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  VulkanTriangle::populateDebugMessengerCreateInfo(
      debugCreateInfo, VulkanTriangle::debugCallback);

  createInfo = vk::InstanceCreateInfo(
      {}, &appInfo, ENABLED_INSTANCE_LAYERS.size(),
      (char const *const *)ENABLED_INSTANCE_LAYERS.data(), exts.size(),
      exts.data(), (vk::DebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo);
#else
  createInfo =
      vk::InstanceCreateInfo({}, &appInfo, {}, {}, exts.size(), exts.data());
#endif

  vk::UniqueInstance instance;
  try {
    instance = vk::createInstanceUnique(createInfo);
  } catch (vk::SystemError const &e) {
    std::cerr << "Failed to create Vulkan instance: " << e.what() << std::endl;
    throw std::runtime_error("Failed to create Vulkan instance");
  }

  if (!mInstance) {
    std::cerr << "Failed to create Vulkan instance." << std::endl;
    throw std::runtime_error("Failed to create Vulkan instance");
  }

  VULKAN_HPP_DEFAULT_DISPATCHER.init(mInstance.get());
  return instance;
}

QueueFamilyIndices
VulkanTriangle::findQueueFamily(vk::PhysicalDevice const &device) const {
  QueueFamilyIndices indices;

  vec<vk::QueueFamilyProperties> queueFamilies =
      device.getQueueFamilyProperties();

  int i = 0;
  for (auto const &family : queueFamilies) {
    if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
      indices.graphicsFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }

    ++i;
  }

  return indices;
}

int VulkanTriangle::rateDevice(vk::PhysicalDevice const &device) const {
  vk::PhysicalDeviceProperties property = device.getProperties();
  vk::PhysicalDeviceFeatures features = device.getFeatures();

  if (!this->findQueueFamily(device).isComplete()) {
    return 0; // Required queue families not found
  }

  if (property.deviceType != vk::PhysicalDeviceType::eDiscreteGpu &&
      property.deviceType != vk::PhysicalDeviceType::eIntegratedGpu) {
    return 0; // Not a suitable device type
  }

  // if (!features.geometryShader || !features.tessellationShader ||
  //     !features.shaderStorageImageWriteWithoutFormat) {
  //   return 0; // Required features not supported
  // }

  if (property.apiVersion < vk::ApiVersion14) {
    std::cerr << "Device API version is too low" << std::endl;
    return 0; // Device API version is too low
  }

  int score = 0;
  score += property.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ? 100
           : property.deviceType == vk::PhysicalDeviceType::eIntegratedGpu
               ? 50
               : 0; // Score based on device type
  score += property.limits.maxImageDimension2D >= 8192 ? 20
           : property.limits.maxImageDimension2D >= 4096
               ? 10
               : 0; // Score based on max image dimension
  score += property.limits.maxComputeWorkGroupCount[0] >= 1024 ? 20
           : property.limits.maxComputeWorkGroupCount[0] >= 512
               ? 10
               : 0; // Score based on max compute work group count
  score += property.limits.maxComputeWorkGroupSize[0] >= 256 ? 20
           : property.limits.maxComputeWorkGroupSize[0] >= 128
               ? 10
               : 0; // Score based on max compute work group size
  score += property.limits.maxComputeSharedMemorySize >= 65536 ? 20
           : property.limits.maxComputeSharedMemorySize >= 32768
               ? 10
               : 0; // Score based on max compute shared memory size

  score += features.geometryShader ? 10 : 0;     // Score if geometry shader is
                                                 // supported
  score += features.tessellationShader ? 10 : 0; // Score if tess
                                                 // shader is supported
  score += features.shaderStorageImageMultisample ? 10 : 0; // Score if
  // shader storage image multisample is supported
  score += features.shaderStorageImageReadWithoutFormat ? 10 : 0; // Score if
  // shader storage image read without format is supported
  score += features.shaderStorageImageWriteWithoutFormat ? 10 : 0; // Score if
  // shader storage image write without format is supported

  return score;
}

vk::PhysicalDevice
VulkanTriangle::pickPhysicalDevice(vk::UniqueInstance const &instance) const {
  vec<vk::PhysicalDevice> physicalDevices =
      instance->enumeratePhysicalDevices();

  if (physicalDevices.empty()) {
    std::cerr << "No Vulkan physical devices found." << std::endl;
    throw std::runtime_error("No Vulkan physical devices found");
  }

  int bestScore = -1;
  vk::PhysicalDevice bestDevice;
  for (auto const &device : physicalDevices) {
    int score = this->rateDevice(device);
    if (score > bestScore) {
      bestScore = score;
      bestDevice = device;
    }
  }

  if (!bestDevice) {
    std::cerr << "No suitable Vulkan physical device found." << std::endl;
    throw std::runtime_error("No suitable Vulkan physical device found");
  }

  return bestDevice;
}

void VulkanTriangle::initVulkan() {
  VULKAN_HPP_DEFAULT_DISPATCHER.init();
  mInstance = this->createInstance("Vulkan Triangle", TERREATE_ENGINE_VERSION);
  this->setupDebugMessenger();
}

void VulkanTriangle::mainLoop() {
  while (true) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_EVENT_QUIT:
        return;
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
        return;
      }
      default:
        break;
      }
    }
  }
}

void VulkanTriangle::cleanup() {
  if (mWindow) {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
  }
  SDL_Quit();
}

void VulkanTriangle::run() {
  mWindow = this->initWindow(800, 600, "Vulkan Triangle");
  this->initVulkan();
  // this->mainLoop();
  this->cleanup();
}

int main() {
  VulkanTriangle app;
  try {
    app.run();
  } catch (std::exception const &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception occurred." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
