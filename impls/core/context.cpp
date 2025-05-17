#include <core/context.hpp>
#include <exception.hpp>

namespace Terreate::Core {
Context::Context() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

Context::~Context() {
  for (auto instance : mInstances) {
    if (mDebugMessengers.at(instance) != VK_NULL_HANDLE) {
      API::trDestroyDebugUtilsMessengerEXT(
          instance, mDebugMessengers.at(instance), nullptr);
    }
    if (instance != VK_NULL_HANDLE) {
      vkDestroyInstance(instance, nullptr);
    }
  }

  mInstances.clear();
  mDebugMessengers.clear();

  glfwTerminate();
}

Instance Context::createInstance(str const &appName, Version appVersion,
                                 str const &engineName, Version engineVersion,
                                 u32 apiVersion) {
  VkApplicationInfo appInfo =
      API::createAppInfo(appName.c_str(), appVersion, engineName.c_str(),
                         engineVersion, apiVersion);
  VkInstance instance = VK_NULL_HANDLE;
#ifdef TERREATE_DEBUG_BUILD
  auto debuggerInfo = this->createDebugCreateInfo();
  instance = API::createInstance(appInfo, debuggerInfo);
#else
  instance = API::createInstance(appInfo);
#endif
  if (instance == nullptr) {
    throw Exception::InstanceCreationFailure();
  }

  mInstances.push_back(instance);
  mDebugMessengers[instance] = VK_NULL_HANDLE;
  return Instance(instance);
}

void Context::attachDebugger(VkInstance instance,
                             VkDebugUtilsMessengerEXT debugger) {
  mInstances.push_back(instance);
  mDebugMessengers[instance] = debugger;
}
} // namespace Terreate::Core
