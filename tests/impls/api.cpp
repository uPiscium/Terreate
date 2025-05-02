#include <api.hpp>
#include <exception.hpp>

namespace Terreate::API {

DebugObject::DebugObject(VkDebugUtilsObjectNameInfoEXT const &info)
    : objectType(info.objectType), handle(info.objectHandle) {
  if (info.pObjectName) {
    name = info.pObjectName;
  } else {
    name = "Unknown";
  }
}

void Debugger::initDebugMessenger(VkInstance instance) {
#ifdef TERREATE_DEBUG_BUILD
  mInstance = instance;
  if (trCreateDebugUtilsMessengerEXT(instance, &mDebugCreateInfo, nullptr,
                                     &mDebugMessenger) != VK_SUCCESS) {
    throw Exception::DebugMessengerCreationFailure();
  }
#endif
}

VKAPI_ATTR VkBool32 VKAPI_CALL Debugger::debugCallbackWrapper(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
  Debugger *debugger = static_cast<Debugger *>(pUserData);
  bool result = false;
  vec<DebugObject> debugObjects(pCallbackData->pObjects,
                                pCallbackData->pObjects +
                                    pCallbackData->objectCount);
  switch (messageSeverity) {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    result = debugger->verbose(pCallbackData->pMessage,
                               (MessageType)messageType, debugObjects);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    result = debugger->info(pCallbackData->pMessage, (MessageType)messageType,
                            debugObjects);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    result = debugger->warning(pCallbackData->pMessage,
                               (MessageType)messageType, debugObjects);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    result = debugger->error(pCallbackData->pMessage, (MessageType)messageType,
                             debugObjects);
    break;
  default:
    result = false; // Unknown severity is not an error.
    break;
  }
  return static_cast<VkBool32>(result);
}

Debugger::Debugger() {
#ifdef TERREATE_DEBUG_BUILD
  mDebugCreateInfo.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  mDebugCreateInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  mDebugCreateInfo.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  mDebugCreateInfo.pfnUserCallback = Debugger::debugCallbackWrapper;

  if (checkValidationLayerSupport()) {
    mDebugCreateInfo.pUserData = this;
    mDebugCreateInfo.pNext = nullptr;
  } else {
    throw Exception::ValidationLayerNotSupported();
  }
#endif
}

Debugger::~Debugger() {
#ifdef TERREATE_DEBUG_BUILD
  trDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
}

VkDebugUtilsMessengerCreateInfoEXT *Debugger::getCreateInfo() {
  return &mDebugCreateInfo;
}

void VulkanInstance::loadEXTfunctions() {
#ifdef TERREATE_DEBUG_BUILD
  trCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkCreateDebugUtilsMessengerEXT");
  trDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkDestroyDebugUtilsMessengerEXT");

  if (!trCreateDebugUtilsMessengerEXT || !trDestroyDebugUtilsMessengerEXT) {
    throw Exception::DebugMessengerFunctionLoadFailure();
  }
#endif
}

vec<char const *> VulkanInstance::getRequiredExts() {
  uint32_t glfwExtensionCount = 0;
  char const **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<char const *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
#ifdef TERREATE_DEBUG_BUILD
  extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  return extensions;
}

VkApplicationInfo VulkanInstance::createAppInfo(char const *appName,
                                                Version appVersion,
                                                char const *engineName,
                                                Version engineVersion,
                                                u32 apiVersion) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName;
  appInfo.applicationVersion =
      VK_MAKE_VERSION(appVersion.major, appVersion.minor, appVersion.patch);
  appInfo.pEngineName = engineName;
  appInfo.engineVersion = VK_MAKE_VERSION(
      engineVersion.major, engineVersion.minor, engineVersion.patch);
  appInfo.apiVersion = apiVersion;
  return appInfo;
}

VulkanInstance::VulkanInstance(str const &appName, Version appVersion,
                               str const &engineName, Version engineVersion,
                               u32 apiVersion,
                               Core::Resource<Debugger> debugger)
    : mDebugger(debugger) {
  auto appInfo =
      this->createAppInfo(appName.c_str(), appVersion, engineName.c_str(),
                          engineVersion, apiVersion);
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
  createInfo.pNext = nullptr;

  auto exts = getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

#ifdef TERREATE_DEBUG_BUILD
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  createInfo.pNext =
      (VkDebugUtilsMessengerCreateInfoEXT *)mDebugger->getCreateInfo();
#endif

  if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
    throw Exception::InstanceCreationFailure();
  }
  this->loadEXTfunctions();

#ifdef TERREATE_DEBUG_BUILD
  mDebugger->initDebugMessenger(mInstance);
#endif
}

VulkanInstance::VulkanInstance(str const &appName, Version appVersion,
                               Core::Resource<Debugger> debugger)
    : VulkanInstance(appName, appVersion, "No Engine", {1, 0, 0},
                     VK_API_VERSION_1_4, debugger) {}

VulkanInstance::VulkanInstance(str const &appName, Version appVersion,
                               str const &engineName, Version engineVersion,
                               Core::Resource<Debugger> debugger)
    : VulkanInstance(appName, appVersion, engineName, engineVersion,
                     VK_API_VERSION_1_4, debugger) {}

VulkanInstance::~VulkanInstance() { vkDestroyInstance(mInstance, nullptr); }

VkInstance const &VulkanInstance::getInstance() const { return mInstance; }

Core::Resource<Debugger> VulkanInstance::getDebugger() const {
  return mDebugger;
}

bool VulkanInstance::isValidationLayerSupported() const {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (char const *layerName : VALIDATION_LAYERS) {
    bool layerFound = false;

    for (auto const &layerProp : availableLayers) {
      if (strcmp(layerName, layerProp.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      std::cerr << "Validation layer " << layerName << " not found!"
                << std::endl;
      return false;
    }
  }

  return true;
}

void loadEXTfunctions(VkInstance instance) {
#ifdef TERREATE_DEBUG_BUILD
  trCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkCreateDebugUtilsMessengerEXT");
  trDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkDestroyDebugUtilsMessengerEXT");

  if (!trCreateDebugUtilsMessengerEXT || !trDestroyDebugUtilsMessengerEXT) {
    throw Exception::DebugMessengerFunctionLoadFailure();
  }
#endif
}

vec<char const *> getRequiredExts() {
  uint32_t glfwExtensionCount = 0;
  char const **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<char const *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
#ifdef TERREATE_DEBUG_BUILD
  extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  return extensions;
}

bool checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (char const *layerName : VALIDATION_LAYERS) {
    bool layerFound = false;

    for (auto const &layerProp : availableLayers) {
      if (strcmp(layerName, layerProp.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      std::cerr << "Validation layer " << layerName << " not found!"
                << std::endl;
      return false;
    }
  }

  return true;
}

VkApplicationInfo createAppInfo(char const *appName, Version appVersion,
                                char const *engineName, Version engineVersion,
                                u32 apiVersion) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName;
  appInfo.applicationVersion =
      VK_MAKE_VERSION(appVersion.major, appVersion.minor, appVersion.patch);
  appInfo.pEngineName = engineName;
  appInfo.engineVersion = VK_MAKE_VERSION(
      engineVersion.major, engineVersion.minor, engineVersion.patch);
  appInfo.apiVersion = apiVersion;
  return appInfo;
}

VkInstance createInstance(VkApplicationInfo const &appInfo,
                          Debugger *debugger) {
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
  createInfo.pNext = nullptr;

  auto exts = getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

#ifdef TERREATE_DEBUG_BUILD
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  createInfo.pNext =
      (VkDebugUtilsMessengerCreateInfoEXT *)debugger->getCreateInfo();
#endif

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw Exception::InstanceCreationFailure();
  }
  loadEXTfunctions(instance);

#ifdef TERREATE_DEBUG_BUILD
  debugger->initDebugMessenger(instance);
#endif

  return instance;
}

VkInstance createInstance(str const &appName, Version appVersion,
                          Debugger *debugger) {
  VkApplicationInfo appInfo = createAppInfo(
      appName.c_str(), appVersion, "No Engine", {1, 0, 0}, VK_API_VERSION_1_4);
  return createInstance(appInfo, debugger);
}
} // namespace Terreate::API
