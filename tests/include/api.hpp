#pragma once

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <type.hpp>

namespace Terreate::API {
using namespace Terreate::Types;

inline PFN_vkCreateDebugUtilsMessengerEXT trCreateDebugUtilsMessengerEXT =
    nullptr;
inline PFN_vkDestroyDebugUtilsMessengerEXT trDestroyDebugUtilsMessengerEXT =
    nullptr;

struct DebugObject {
public:
  VkObjectType objectType;
  u64 handle;
  str name;

public:
  DebugObject(VkDebugUtilsObjectNameInfoEXT const &info);
};

class Debugger {
private:
  VkDebugUtilsMessengerCreateInfoEXT mDebugCreateInfo;
  VkDebugUtilsMessengerEXT mDebugMessenger;
  VkInstance mInstance;

  friend VkInstance createInstance(str const &appName, u32 appVersion);

public:
  void initDebugMessenger(VkInstance instance);

public:
  enum class MessageType {
    GENERAL = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
    VALIDATION = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
    PERFORMANCE = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    DEVICE_ADDRESS_BINDING =
        VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
  };

public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackWrapper(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
      void *pUserData);

public:
  Debugger();
  ~Debugger();

  VkDebugUtilsMessengerCreateInfoEXT *getCreateInfo();

  virtual bool verbose(str const &message, MessageType const messageType,
                       vec<DebugObject> const &objects) = 0;
  virtual bool info(str const &message, MessageType const messageType,
                    vec<DebugObject> const &objects) = 0;
  virtual bool warning(str const &message, MessageType const messageType,
                       vec<DebugObject> const &objects) = 0;
  virtual bool error(str const &message, MessageType const messageType,
                     vec<DebugObject> const &objects) = 0;
};

void loadEXTfunctions(VkInstance instance);
vec<char const *> getRequiredExts();
bool checkValidationLayerSupport();
VkInstance createInstance(VkApplicationInfo const &appInfo, Debugger *debugger);
VkInstance createInstance(str const &appName, Version appVersion,
                          Debugger *debugger);
} // namespace Terreate::API
