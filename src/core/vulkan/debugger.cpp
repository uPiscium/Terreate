#include "core/vulkan/debugger.hpp"

#include <iostream>

namespace Terreate::Core::Vulkan {

DebugObject::DebugObject(VkDebugUtilsObjectNameInfoEXT const &info)
    : objectType(info.objectType), handle(info.objectHandle) {
  if (info.pObjectName) {
    name = info.pObjectName;
  } else {
    name = "Unknown";
  }
}

bool IDebugger::verbose(str const &message, MessageType const type,
                        vec<DebugObject> const &object) {
  return false;
}

bool IDebugger::info(str const &message, MessageType const type,
                     vec<DebugObject> const &object) {
  return false;
}

bool IDebugger::warning(str const &message, MessageType const type,
                        vec<DebugObject> const &object) {
  return false;
}

bool IDebugger::error(str const &message, MessageType const type,
                      vec<DebugObject> const &object) {
  return false;
}

bool DefaultDebugger::warning(str const &message, MessageType const type,
                              vec<DebugObject> const &object) {
  std::cerr << "[VULKAN WARNING] " << message << std::endl;
  return false;
}

bool DefaultDebugger::error(str const &message, MessageType const type,
                            vec<DebugObject> const &object) {
  std::cerr << "[VULKAN ERROR] " << message << std::endl;
  return false;
}

VkDebugUtilsMessengerEXT createDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
    VkAllocationCallbacks const *pAllocator) {
  if (instance == VK_NULL_HANDLE) {
    throw DebuggerException("Invalid Vulkan instance provided.");
  }
  if (pCreateInfo == nullptr) {
    throw DebuggerException("Invalid create info provided.");
  }

  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (!func) {
    throw DebuggerException(
        "Failed to get address of vkCreateDebugUtilsMessengerEXT.");
  }

  VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
  if (func(instance, pCreateInfo, nullptr, &messenger) != VK_SUCCESS) {
    throw DebuggerException("Failed to create debug utils messenger.");
  }

  return messenger;
}

void destroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT messenger,
                                   VkAllocationCallbacks const *pAllocator) {
  if (instance == VK_NULL_HANDLE) {
    throw DebuggerException("Invalid Vulkan instance provided.");
  }
  if (messenger == VK_NULL_HANDLE) {
    throw DebuggerException("Invalid debug utils messenger provided.");
  }

  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (!func) {
    throw DebuggerException(
        "Failed to get address of vkDestroyDebugUtilsMessengerEXT.");
  }

  func(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallbackWrapper(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                     VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                     void *pUserData) {
  if (!pUserData) {
    return VK_FALSE;
  }

  IDebugger *debugger = static_cast<IDebugger *>(pUserData);
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

} // namespace Terreate::Core::Vulkan
