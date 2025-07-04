#include "../../../include/module/sdl/cursor.hpp"

#include <cstring>

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
} // namespace Terreate::SDL
