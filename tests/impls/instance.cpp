#include <instance.hpp>

namespace Terreate::Core {
VkDebugUtilsMessengerCreateInfoEXT
VulkanInstance::createDebugCreateInfo() const {
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallbackWrapper;
  createInfo.pUserData = nullptr;
  createInfo.pNext = nullptr;
  return createInfo;
}

VulkanInstance::VulkanInstance()
    : mInstance(nullptr), mDebugMessenger(nullptr) {}

VulkanInstance::VulkanInstance(str const &appName, Version appVersion,
                               str const &engineName, Version engineVersion,
                               u32 apiVersion) {
#ifdef TERREATE_DEBUG_BUILD
  auto debuggerInfo = this->createDebugCreateInfo();
  mInstance = API::createInstance(
      API::createAppInfo(appName.c_str(), appVersion, engineName.c_str(),
                         engineVersion, apiVersion),
      debuggerInfo);
#else
  mInstance = API::createInstance(
      API::createAppInfo(appName.c_str(), appVersion, engineName.c_str(),
                         engineVersion, apiVersion));
#endif
  if (mInstance == nullptr) {
    throw Exception::InstanceCreationFailure();
  }
}

VulkanInstance::~VulkanInstance() {
#ifdef TERREATE_DEBUG_BUILD
  if (mDebugMessenger) {
    API::trDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
  }
#endif

  if (mLogicalDevice) {
    mLogicalDevice.dispose();
  }

  for (auto &window : mWindows) {
    window.dispose();
  }

  if (mInstance) {
    vkDestroyInstance(mInstance, nullptr);
  }
}

void VulkanInstance::attachDebugger(IDebugger *debugger) {
#ifdef TERREATE_DEBUG_BUILD
  if (!debugger)
    return;

  if (API::checkValidationLayerSupport()) {
    auto debuggerInfo = this->createDebugCreateInfo();
    debuggerInfo.pUserData = debugger;
    debuggerInfo.pNext = nullptr;
    if (API::trCreateDebugUtilsMessengerEXT(mInstance, &debuggerInfo, nullptr,
                                            &mDebugMessenger) != VK_SUCCESS) {
      throw Exception::DebugMessengerCreationFailure();
    }
  } else {
    throw Exception::ValidationLayerNotSupported();
  }
#endif
}

RefPointer<Window> VulkanInstance::createWindow(str const &title, u32 width,
                                                u32 height,
                                                WindowSettings settings) {
  auto window =
      HubPointer<Window>::create(mInstance, title, width, height, settings);
  auto ref = window.ref();
  mWindows.push_back(std::move(window));

  if (!mLogicalDevice) {
    mLogicalDevice = HubPointer<LogicalDevice>(ref->createDevice());
  }

  ref->createSwapchain(mLogicalDevice.ref());
  return ref;
}
} // namespace Terreate::Core
