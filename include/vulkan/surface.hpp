#pragma once
#include "vkobj.hpp"
#include "window.hpp"

#include "../common/type.hpp"

namespace Terreate::Vulkan {
class Surface {
private:
  PROHIBIT_COPY_AND_ASSIGN(Surface);

private:
  VkInstance mInstance = VK_NULL_HANDLE;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;

public:
  Surface(VkInstance instance, VkObjectRef<Window> window);
  ~Surface();

  operator VkSurfaceKHR() const { return mSurface; }
};
} // namespace Terreate::Vulkan
