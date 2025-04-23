#include <api.hpp>
#include <exception.hpp>

namespace Terreate::API {

DebugObject::DebugObject(VkDebugUtilsObjectNameInfoEXT const &info)
    : objectType(info.objectType), handle(info.objectHandle),
      name(info.pObjectName) {}

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
  // std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
  // Debugger *debugger = static_cast<Debugger *>(pUserData);
  // bool result = false;
  // vec<DebugObject> debugObjects(pCallbackData->pObjects,
  //                               pCallbackData->pObjects +
  //                                   pCallbackData->objectCount);
  // switch (messageSeverity) {
  // case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
  //   result = debugger->verbose(pCallbackData->pMessage,
  //                              (MessageType)messageType, debugObjects);
  //   break;
  // case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
  //   result = debugger->info(pCallbackData->pMessage,
  //   (MessageType)messageType,
  //                           debugObjects);
  //   break;
  // case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
  //   result = debugger->warning(pCallbackData->pMessage,
  //                              (MessageType)messageType, debugObjects);
  //   break;
  // case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
  //   result = debugger->error(pCallbackData->pMessage,
  //   (MessageType)messageType,
  //                            debugObjects);
  //   break;
  // default:
  //   result = false;
  //   break;
  // }
  // return static_cast<VkBool32>(result);
}

VkDebugUtilsMessengerCreateInfoEXT Debugger::createDebuggerInfo() {
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = Debugger::debugCallbackWrapper;
  return createInfo;
}

Debugger::Debugger() {
  mDebugCreateInfo = Debugger::createDebuggerInfo();

  if (checkValidationLayerSupport()) {
    mDebugCreateInfo.pUserData = this;
    mDebugCreateInfo.pNext = nullptr;
  } else {
    throw Exception::ValidationLayerNotSupported();
  }
}

Debugger::~Debugger() {
#ifdef TERREATE_DEBUG_BUILD
  trDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
}

VkDebugUtilsMessengerCreateInfoEXT *Debugger::getCreateInfo() {
  return &mDebugCreateInfo;
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

u32 makeVersion(u32 const major, u32 const minor, u32 const patch) {
  return VK_MAKE_VERSION(major, minor, patch);
}

VkApplicationInfo createAppInfo(str const &appName, u32 const appVersion,
                                str const &engineName, u32 const engineVersion,
                                u32 const apiVersion) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

  appInfo.pApplicationName = appName.c_str();
  appInfo.applicationVersion = appVersion;
  appInfo.pEngineName = engineName.c_str();
  appInfo.engineVersion = engineVersion;
  appInfo.apiVersion = apiVersion;
  return appInfo;
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

VkInstance createInstance(VkApplicationInfo const &appInfo,
                          Debugger *debugger) {
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
  createInfo.pNext = nullptr;

#ifdef TERREATE_DEBUG_BUILD
  auto debuggerInfo = Debugger::createDebuggerInfo();
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debuggerInfo;
#endif

  auto exts = getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw Exception::InstanceCreationFailure();
  }

#ifdef TERREATE_DEBUG_BUILD
  debugger->initDebugMessenger(instance);
#endif

  loadEXTfunctions(instance);
  return instance;
}

VkInstance createInstance(str const &appName, u32 appVersion,
                          Debugger *debugger) {
  auto appInfo = createAppInfo(appName, appVersion);
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
  createInfo.pNext = nullptr;

#ifdef TERREATE_DEBUG_BUILD
  auto debuggerInfo = Debugger::createDebuggerInfo();
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
  createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debuggerInfo;
#endif

  auto exts = getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw Exception::InstanceCreationFailure();
  }

#ifdef TERREATE_DEBUG_BUILD
  debugger->initDebugMessenger(instance);
#endif

  loadEXTfunctions(instance);
  return instance;
}
} // namespace Terreate::API
