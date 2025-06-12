#include "../../include/vulkan/surface.hpp"

namespace Terreate::Vulkan {
Surface::Surface(VkInstance instance, VkObjectRef<IWindow> window)
    : mInstance(instance) {
  if (glfwCreateWindowSurface(instance, *window, nullptr, &mSurface) !=
      VK_SUCCESS) {
    char const *log = nullptr;
    glfwGetError(&log);
    throw Exception::SurfaceCreationFailure(log);
  }
}

Surface::~Surface() {
  if (mSurface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    mSurface = VK_NULL_HANDLE;
  }
}

} // namespace Terreate::Vulkan
