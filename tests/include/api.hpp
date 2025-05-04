#pragma once

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <debugger.hpp>
#include <resource.hpp>
#include <type.hpp>

namespace Terreate::API {
using namespace Terreate::Type;

inline PFN_vkCreateDebugUtilsMessengerEXT trCreateDebugUtilsMessengerEXT =
    nullptr;
inline PFN_vkDestroyDebugUtilsMessengerEXT trDestroyDebugUtilsMessengerEXT =
    nullptr;

class VulkanInstance {
private:
  VkInstance mInstance;
  shared<Debugger> mDebugger;

private:
  void loadEXTfunctions();
  vec<char const *> getRequiredExts();
  VkApplicationInfo createAppInfo(char const *appName, Version appVersion,
                                  char const *engineName, Version engineVersion,
                                  u32 apiVersion);

public:
  VulkanInstance(str const &appName, Version appVersion, str const &engineName,
                 Version engineVersion, u32 apiVersion,
                 shared<Debugger> debugger = nullptr);
  VulkanInstance(str const &appName, Version appVersion, str const &engineName,
                 Version engineVersion, shared<Debugger> debugger = nullptr);
  VulkanInstance(str const &appName, Version appVersion,
                 shared<Debugger> debugger = nullptr);
  ~VulkanInstance();

  VkInstance const &getInstance() const;
  shared<Debugger> getDebugger() const;

  bool isValidationLayerSupported() const;

  void initInstance();
};

void loadEXTfunctions(VkInstance instance);
vec<char const *> getRequiredExts();
bool checkValidationLayerSupport();
VkApplicationInfo createAppInfo(char const *appName, Version appVersion,
                                char const *engineName = "No Engine",
                                Version engineVersion = {1, 0, 0},
                                u32 apiVersion = VK_API_VERSION_1_4);
VkInstance createInstance(VkApplicationInfo const &appInfo, Debugger *debugger);
VkInstance createInstance(str const &appName, Version appVersion,
                          Debugger *debugger);
} // namespace Terreate::API
