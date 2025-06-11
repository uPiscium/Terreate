#pragma once
#include "vkobj.hpp"
#include "window.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class Surface {
private:
  PROHIBIT_COPY_AND_ASSIGN(Surface);

private:
  VkInstance mInstance = VK_NULL_HANDLE;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;

public:
  Surface(VkInstance instance, VkObjectRef<Window> window);
  ~Surface() { this->dispose(); }

  void dispose();

  operator VkSurfaceKHR() const { return mSurface; }
};
} // namespace Terreate::Core
