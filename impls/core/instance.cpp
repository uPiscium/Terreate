#include <core/instance.hpp>
#include <utils/exception.hpp>

namespace Terreate::Core {
Instance::Instance(char const *appName, vec<char const *> const &exts) {
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName;
  appInfo.applicationVersion = 1;

  VkInstanceCreateInfo instanceInfo = {};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.ppEnabledExtensionNames = exts.data();
  instanceInfo.enabledExtensionCount = exts.size();

  VkResult result = vkCreateInstance(&instanceInfo, nullptr, &mInstance);

  if (result != VK_SUCCESS) {
    switch (result) {
    case VK_ERROR_OUT_OF_HOST_MEMORY:
      throw Exception::InstanceCreationFailure("Out of host memory");
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
      throw Exception::InstanceCreationFailure("Out of device memory");
    case VK_ERROR_INITIALIZATION_FAILED:
      throw Exception::InstanceCreationFailure("Initialization failed");
    case VK_ERROR_LAYER_NOT_PRESENT:
      throw Exception::InstanceCreationFailure("Layer not present");
    case VK_ERROR_EXTENSION_NOT_PRESENT:
      throw Exception::InstanceCreationFailure("Extension not present");
    case VK_ERROR_INCOMPATIBLE_DRIVER:
      throw Exception::InstanceCreationFailure("Incompatible driver");
    default:
      str msg = "Unknown error at:" + LOCATION_INFO();
      throw Exception::FatalException(msg);
    }
  }
}

Instance::~Instance() {
  if (mInstance != VK_NULL_HANDLE) {
    vkDestroyInstance(mInstance, nullptr);
    mInstance = VK_NULL_HANDLE;
  }
}

} // namespace Terreate::Core
