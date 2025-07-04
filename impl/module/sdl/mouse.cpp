#include "../../../include/module/sdl/mouse.hpp"

namespace Terreate::SDL {
bool Mouse::getMouseButton(MouseButton const &button) const {
  i32 state = SDL_GetMouseState(nullptr, nullptr);
  return (state & SDL_BUTTON_MASK((i32)button)) != 0;
}

pair<float> Mouse::getCursorPosition() const {
  float x, y;
  SDL_GetMouseState(&x, &y);
  return {x, y};
}
} // namespace Terreate::SDL
