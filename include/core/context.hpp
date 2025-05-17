#pragma once
#include <core/api.hpp>
#include <core/instance.hpp>
#include <type.hpp>
#include <util/debugger.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

class Context {
private:
  vec<VkInstance> mInstances;
  map<VkInstance, VkDebugUtilsMessengerEXT> mDebugMessengers;

public:
  Context();
  ~Context();

  Instance createInstance(str const &appName, Version appVersion,
                          str const &engineName = "Terreate",
                          Version engineVersion = TERREATE_VERSION,
                          u32 apiVersion = VK_API_VERSION_1_4);

  void attachDebugger(VkInstance instance, VkDebugUtilsMessengerEXT debugger);
};
} // namespace Terreate::Core
