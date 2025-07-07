#include "../../../include/module/sdl/mouse.hpp"

namespace Terreate::SDL {
Cursor::~Cursor() {
  SDL_DestroyCursor(mCursor);
  SDL_DestroySurface(mCursorImage);
}

void Cursor::setImage(u32 const &width, u32 const &height,
                      ubyte const *pixels) {
  mCursorImage = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
  mCursor = SDL_CreateColorCursor(mCursorImage, mXHot, mYHot);
}

bool Mouse::getMouseButton(MouseButton const &button) const {
  i32 state = SDL_GetMouseState(nullptr, nullptr);
  return (state & SDL_BUTTON_MASK((i32)button)) != 0;
}

vec2 Mouse::getCursorPosition() const {
  float x, y;
  SDL_GetMouseState(&x, &y);
  return {x, y};
}

vec2 Mouse::getGlobalCursorPosition() const {
  float x, y;
  SDL_GetGlobalMouseState(&x, &y);
  return {x, y};
}

vec2 Mouse::getRelativeCursorPosition() const {
  float x, y;
  SDL_GetRelativeMouseState(&x, &y);
  return {x, y};
}

vec<SDL_MouseID> Mouse::getMise() {
  i32 numMice;
  SDL_MouseID *ids = SDL_GetMice(&numMice);
  vec<SDL_MouseID> mice(ids, ids + numMice);
  SDL_free(ids);
  return mice;
}
} // namespace Terreate::SDL
