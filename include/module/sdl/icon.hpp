#pragma once

#include "../common/type.hpp"

namespace Terreate::SDL {
class Icon {
private:
  PROHIBIT_COPY_AND_ASSIGN(Icon);

private:
  SDL_Surface *mIconImage = nullptr;

public:
  Icon() : mIconImage(nullptr) {}
  ~Icon() { SDL_DestroySurface(mIconImage); }

  void setImage(u32 const &width, u32 const &height, ubyte const *pixels) {
    mIconImage = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
  }

  operator SDL_Surface *() const { return mIconImage; }
};
} // namespace Terreate::SDL
