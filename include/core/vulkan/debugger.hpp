#pragma once

#include "common/type.hpp"

#include "enum.hpp"
#include "exception.hpp"

namespace Terreate::Core::Vulkan {

struct DebugObject {
public:
  VkObjectType objectType;
  u64 handle;
  str name;

public:
  DebugObject(VkDebugUtilsObjectNameInfoEXT const &info);
};

class IDebugger {
public:
  virtual ~IDebugger() = default;

  virtual bool verbose(str const &message, MessageType const type,
                       vec<DebugObject> const &object);
  virtual bool info(str const &message, MessageType const type,
                    vec<DebugObject> const &object);
  virtual bool warning(str const &message, MessageType const type,
                       vec<DebugObject> const &object);
  virtual bool error(str const &message, MessageType const type,
                     vec<DebugObject> const &object);
};

class DefaultDebugger : public IDebugger {
public:
  bool warning(str const &message, MessageType const type,
               vec<DebugObject> const &object) override;
  bool error(str const &message, MessageType const type,
             vec<DebugObject> const &object) override;
};

VkDebugUtilsMessengerEXT createDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
    VkAllocationCallbacks const *pAllocator);
void destroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT messenger,
                                   VkAllocationCallbacks const *pAllocator);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackWrapper(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void *pUserData);

} // namespace Terreate::Core::Vulkan
