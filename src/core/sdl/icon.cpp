#include "core/sdl/exception.hpp"
#include "core/sdl/icon.hpp"

namespace Terreate::Core::SDL {

Icon::~Icon() {
  if (mIcon) {
    SDL_DestroySurface(mIcon);
    mIcon = nullptr;
  }
}

void Icon::setImage(pair<u32> const &size, vec<u8> const &pixels) {
  if (mIcon) {
    SDL_DestroySurface(mIcon);
    mIcon = nullptr;
  }

  mIcon = SDL_CreateSurfaceFrom(size.first, size.second, SDL_PIXELFORMAT_RGBA32,
                                (void *)pixels.data(), size.first * 4);
  if (!mIcon) {
    throw IconException("Failed to create icon surface: " +
                        str(SDL_GetError()));
  }
}

Icon::operator SDL_Surface *() const { return mIcon; }

} // namespace Terreate::Core::SDL
