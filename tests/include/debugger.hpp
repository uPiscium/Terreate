#pragma once
#include <api.hpp>
#include <type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

struct DebugObject {
public:
  VkObjectType objectType;
  u64 handle;
  str name;

public:
  DebugObject(VkDebugUtilsObjectNameInfoEXT const &info);
};

enum class MessageType {
  GENERAL = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
  VALIDATION = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
  PERFORMANCE = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
  DEVICE_ADDRESS_BINDING =
      VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
};

class IDebugger {
public:
  virtual ~IDebugger() = default;

  virtual bool verbose(str const &message, MessageType const type,
                       vec<DebugObject> const &object) = 0;
  virtual bool info(str const &message, MessageType const type,
                    vec<DebugObject> const &object) = 0;
  virtual bool warning(str const &message, MessageType const type,
                       vec<DebugObject> const &object) = 0;
  virtual bool error(str const &message, MessageType const type,
                     vec<DebugObject> const &object) = 0;
};

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackWrapper(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void *pUserData);

} // namespace Terreate::Core
