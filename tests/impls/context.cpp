#include "hubptr.hpp"
#include <context.hpp>

namespace Terreate::Core {
TerreateContext::TerreateContext() {
  // Initialize GLFW
  if (!glfwInit()) {
    throw Exception::GLFWInitializeFailure();
  }

  // Set GLFW to not create an OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

TerreateContext::~TerreateContext() {
  // Destroy all Vulkan instances
  for (auto &instance : mInstances) {
    instance.dispose();
  }

  // Terminate GLFW
  glfwTerminate();
}

RefPointer<VulkanInstance>
TerreateContext::createInstance(str const &appName, Version appVersion,
                                str const &engineName, Version engineVersion,
                                u32 apiVersion) {
  auto instance = HubPointer<VulkanInstance>::create(
      appName, appVersion, engineName, engineVersion, apiVersion);
  auto ref = instance.ref();
  mInstances.push_back(std::move(instance));
  return ref;
}
} // namespace Terreate::Core
