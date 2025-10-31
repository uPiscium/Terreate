#pragma once

#include "common/type.hpp"

namespace Terreate::SDL {

class Window {
private:
  PROHIBIT_COPY_AND_ASSIGN(Window);

private:
  SDL_Window *mWindow = nullptr;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;

public:
  Window(int const &width, int const &height, str const &title);
  ~Window();

  ;
};

} // namespace Terreate::SDL
