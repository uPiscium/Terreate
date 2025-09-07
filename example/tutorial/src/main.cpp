#include "../include/main.hpp"

void VulkanTutorial::initWindow() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_CAMERA |
                SDL_INIT_SENSOR | SDL_INIT_AUDIO)) {
    throw std::runtime_error("Failed to initialize SDL.");
    return;
  }

  mWindow = SDL_CreateWindow("Vulkan Tutorial", WINDOW_WIDTH, WINDOW_HEIGHT,
                             SDL_WINDOW_VULKAN);
}

bool VulkanTutorial::checkValidationLayerSupport() {
  u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  // std::cout << "Available Vulkan layers:\n";
  // for (const auto &layer : availableLayers) {
  //   std::cout << '\t' << layer.layerName << '\n';
  // }

  for (auto const &layer : VALIDATION_LAYERS) {
    bool layerFound = false;

    for (const auto &availableLayer : availableLayers) {
      if (std::strcmp(layer, availableLayer.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

vec<char const *> VulkanTutorial::getRequiredExtensions() {
  u32 extensionCount = 0;
  char const *const *extensions =
      SDL_Vulkan_GetInstanceExtensions(&extensionCount);
  if (!extensions) {
    throw std::runtime_error("Failed to get required Vulkan extensions.");
  }
  vec<char const *> requiredExtensions(extensions, extensions + extensionCount);

  if (mDebugMode) {
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return requiredExtensions;
}

void VulkanTutorial::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr; // Optional
}

void VulkanTutorial::createInstance(str const &appName, u32 const &appVersion) {
  if (mDebugMode && !this->checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested, but not available.");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName.c_str();
  appInfo.applicationVersion = appVersion;
  appInfo.pEngineName = ENGINE_NAME;
  appInfo.engineVersion = ENGINE_VERSION;
  appInfo.apiVersion = VK_API_VERSION_1_4;

  vec<char const *> extensions = this->getRequiredExtensions();

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (mDebugMode) {
    createInfo.enabledLayerCount =
        sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]);
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;

    this->populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  u32 extCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
  vec<VkExtensionProperties> availableExtensions(extCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount,
                                         availableExtensions.data());

  // std::cout << "Available Vulkan extensions:\n";
  // for (const auto &extension : availableExtensions) {
  //   std::cout << '\t' << extension.extensionName << '\n';
  // }

  if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan instance.");
  }
}

VkResult VulkanTutorial::createDebugUtilsMessengerEXT(
    VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
    VkAllocationCallbacks const *pAllocator) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      mInstance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(mInstance, pCreateInfo, nullptr, &mDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VkResult VulkanTutorial::destroyDebugUtilsMessengerEXT(
    VkAllocationCallbacks const *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      mInstance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr && mDebugMessenger != VK_NULL_HANDLE) {
    func(mInstance, mDebugMessenger, nullptr);
    mDebugMessenger = VK_NULL_HANDLE;
    return VK_SUCCESS;
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void VulkanTutorial::setupDebugMessenger() {
  if (!mDebugMode) {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  this->populateDebugMessengerCreateInfo(createInfo);

  if (this->createDebugUtilsMessengerEXT(&createInfo, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to set up debug messenger.");
  }
}

void VulkanTutorial::initVulkan() {
  this->createInstance("Vulkan Tutorial", VK_MAKE_VERSION(0, 1, 0));
  this->setupDebugMessenger();
}

bool VulkanTutorial::pollEvents() {
  SDL_Event handle;
  while (SDL_PollEvent(&handle)) {
    switch (handle.type) {
    case SDL_EVENT_QUIT:
      return false;

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    case SDL_EVENT_WINDOW_DESTROYED:
      return false;

    default:
      break;
    }
  }

  return true;
}

void VulkanTutorial::mainLoop() {
  bool loop = true;
  while (loop) {
    loop = this->pollEvents();
  }
}

void VulkanTutorial::cleanup() {
  if (mDebugMode) {
    this->destroyDebugUtilsMessengerEXT(nullptr);
  }

  if (mInstance != VK_NULL_HANDLE) {
    vkDestroyInstance(mInstance, nullptr);
    mInstance = VK_NULL_HANDLE;
  }

  if (mWindow) {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
  }

  SDL_Quit();
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanTutorial::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
  switch (messageSeverity) {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    std::cerr << "VERBOSE: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    std::cerr << "INFO: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    std::cerr << "WARNING: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    std::cerr << "ERROR: ";
    break;
  default:
    std::cerr << "UNKNOWN: ";
    break;
  }
  std::cerr << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

void VulkanTutorial::run() {
  this->initWindow();
  this->initVulkan();
  this->mainLoop();
  this->cleanup();
}

int main() {
  VulkanTutorial app(true);

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
