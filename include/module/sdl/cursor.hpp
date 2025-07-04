#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

namespace Terreate::SDL {
class Cursor {
private:
  PROHIBIT_COPY_AND_ASSIGN(Cursor);

private:
  SDL_Surface *mCursorImage = nullptr;
  SDL_Cursor *mCursor = nullptr;
  i32 mXHot = 0;
  i32 mYHot = 0;

public:
  Cursor(i32 const &xHot = 0, i32 const &yHot = 0) : mXHot(xHot), mYHot(yHot) {}
  ~Cursor();

  void setImage(u32 const &width, u32 const &height, ubyte const *pixels);

  operator SDL_Cursor *() const { return mCursor; }
  operator bool() const { return mCursor != nullptr; }
};

class SystemCursor {
private:
  PROHIBIT_COPY_AND_ASSIGN(SystemCursor);

private:
  SDL_Cursor *mCursor = nullptr;

public:
  SystemCursor(CursorShape const &shape) {
    mCursor = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(shape));
  }
  ~SystemCursor() { SDL_DestroyCursor(mCursor); }

  operator SDL_Cursor *() const { return mCursor; }
  operator bool() const { return mCursor != nullptr; }
};
} // namespace Terreate::SDL
