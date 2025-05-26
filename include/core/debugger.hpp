#pragma once
#include "../decl/enum.hpp"
#include "../decl/type.hpp"

namespace Terreate::Core {
struct DebugObject {
public:
  VkObjectType objectType;
  Type::u64 handle;
  Type::str name;

public:
  DebugObject(VkDebugUtilsObjectNameInfoEXT const &info);
};

class IDebugger {
public:
  virtual ~IDebugger() = default;

  virtual bool verbose(Type::str const &message, Type::MessageType const type,
                       Type::vec<DebugObject> const &object) = 0;
  virtual bool info(Type::str const &message, Type::MessageType const type,
                    Type::vec<DebugObject> const &object) = 0;
  virtual bool warning(Type::str const &message, Type::MessageType const type,
                       Type::vec<DebugObject> const &object) = 0;
  virtual bool error(Type::str const &message, Type::MessageType const type,
                     Type::vec<DebugObject> const &object) = 0;
};

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackWrapper(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void *pUserData);

} // namespace Terreate::Core
