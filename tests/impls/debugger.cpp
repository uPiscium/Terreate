#include <debugger.hpp>

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

} // namespace Terreate::API
