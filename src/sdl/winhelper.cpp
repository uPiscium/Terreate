#include <sdl/exception.hpp>
#include <sdl/window.hpp>
#include <sdl/winhelper.hpp>

namespace Terreate::SDL {
pair<i32> WindowProperty::getPosition() const {
  i32 xpos, ypos;
  SDL_GetWindowPosition(mWindow, &xpos, &ypos);
  return pair<i32>(xpos, ypos);
}

pair<u32> WindowProperty::getSize() const {
  i32 width, height;
  SDL_GetWindowSize(mWindow, &width, &height);
  return pair<u32>(static_cast<u32>(width), static_cast<u32>(height));
}

pair<u32> WindowProperty::getFramebufferSize() const {
  i32 width, height;
  SDL_GetWindowSizeInPixels(mWindow, &width, &height);
  return pair<u32>{static_cast<u32>(width), static_cast<u32>(height)};
}
} // namespace Terreate::SDL
