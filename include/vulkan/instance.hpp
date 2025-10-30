#pragma once

#include "common/type.hpp"

#include "debugger.hpp"

namespace Terreate::Vulkan {

class Instance {
private:
  PROHIBIT_COPY_AND_ASSIGN(Instance);

private:
  constexpr static Version const ENGINE_VERSION = Version(0, 1, 0);
  constexpr static char const *ENGINE_NAME = "Terreate";

private:
  bool mDebugMode = false;
  vec<str> mEnabledExtensions;
  vec<str> mEnabledLayers;
  VkInstance mHandle = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;

private:
  bool checkValidationLayerSupport(vec<str> const &layers) const;
  vec<char const *> getRequiredExtensions(vec<str> const &exts) const;
  VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo() const;

private:
  Instance(str const &appName, Version const &appVersion,
           bool debugMode = false, vec<str> const &extensions = {},
           vec<str> const &layers = {"VK_LAYER_KHRONOS_validation"});

public:
  ~Instance();

  void attachDebugger(shared<IDebugger> debugger);

  [[nodiscard]] operator VkInstance() const;

public:
  static shared<Instance>
  create(str const &appName, Version const &appVersion, bool debugMode = false,
         vec<str> const &extensions = {},
         vec<str> const &layers = {"VK_LAYER_KHRONOS_validation"});
};

} // namespace Terreate::Vulkan
