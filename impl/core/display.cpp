#include "../../include/core/display.hpp"

namespace Terreate::Core {
Display::Display(VkInstance instance, Vulkan::VkObject<Vulkan::Device> *device,
                 Type::str const &title, Type::pair<Type::i32> const &size,
                 Vulkan::WindowSettings const &settings) {
  mWindow =
      Vulkan::makeVkObject<Vulkan::Window>(instance, title, size, settings);
  mSurface = Vulkan::makeVkObject<Vulkan::Surface>(instance, mWindow.ref());

  if (!*device) {
    *device = Vulkan::makeVkObject<Vulkan::Device>(instance, mSurface.ref());
  }

  mSwapchain = Vulkan::makeVkObject<Vulkan::Swapchain>(
      device->ref(), mWindow->properties.framebufferSize, mSurface.ref());
}

Display::~Display() {
  mSwapchain.reset();
  mSurface.reset();
  mWindow.reset();
}

} // namespace Terreate::Core
