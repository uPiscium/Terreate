#include "../../include/core/surface.hpp"

namespace Terreate::Core {
Surface::Surface(VkInstance instance, VkObjectRef<Window> window)
    : mInstance(instance) {
  if (glfwCreateWindowSurface(instance, *window, nullptr, &mSurface) !=
      VK_SUCCESS) {
    char const *log = nullptr;
    glfwGetError(&log);
    throw Exception::SurfaceCreationFailure(log);
  }
}

void Surface::dispose() {
  if (mSurface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    mSurface = VK_NULL_HANDLE;
  }
}

} // namespace Terreate::Core
