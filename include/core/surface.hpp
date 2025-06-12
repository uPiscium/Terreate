#pragma once
#include "vkobj.hpp"
#include "window.hpp"

#include "../common/type.hpp"

namespace Terreate::Core {
class ISurface {
public:
  virtual ~ISurface() = default;
  virtual operator VkSurfaceKHR() const = 0;
};

class Surface : public ISurface {
private:
  PROHIBIT_COPY_AND_ASSIGN(Surface);

private:
  VkInstance mInstance = VK_NULL_HANDLE;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;

public:
  Surface(VkInstance instance, VkObjectRef<IWindow> window);
  ~Surface() override;

  operator VkSurfaceKHR() const override { return mSurface; }
};
} // namespace Terreate::Core
