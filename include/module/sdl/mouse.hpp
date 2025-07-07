#pragma once

#include "../common/enum.hpp"
#include "../common/math.hpp"
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

class Mouse {
private:
  PROHIBIT_COPY_AND_ASSIGN(Mouse);

private:
  SDL_MouseID mMouseID = 0;

public:
  Mouse(SDL_MouseID const &mouseID) : mMouseID(mouseID) {}
  ~Mouse() = default;

  str getName() const {
    return mMouseID != 0 ? SDL_GetMouseNameForID(mMouseID)
                         : "SYSTEM SHARED MOUSE";
  }
  bool getMouseButton(MouseButton const &button) const;
  vec2 getCursorPosition() const;
  vec2 getGlobalCursorPosition() const;
  vec2 getRelativeCursorPosition() const;

  void setCursor(Cursor const &cursor) const {
    SDL_SetCursor((SDL_Cursor *)cursor);
  }
  void setCursor(SystemCursor const &cursor) const {
    SDL_SetCursor((SDL_Cursor *)cursor);
  }
  void setCursorPosition(float const &x, float const &y) const {
    SDL_WarpMouseInWindow(SDL_GetMouseFocus(), x, y);
  }
  void setCursorPosition(vec2 const &pos) const {
    SDL_WarpMouseInWindow(SDL_GetMouseFocus(), pos.x, pos.y);
  }
  void setGlobalCursorPosition(float const &x, float const &y) const {
    SDL_WarpMouseGlobal(x, y);
  }
  void setGlobalCursorPosition(vec2 const &pos) const {
    SDL_WarpMouseGlobal(pos.x, pos.y);
  }

  void show() const { SDL_ShowCursor(); }
  void hide() const { SDL_HideCursor(); }
  void capture(bool const &capture) const { SDL_CaptureMouse(capture); }

public:
  static vec<SDL_MouseID> getMise();
  static bool isCursorVisible() { return SDL_CursorVisible(); }
  static bool hasMouse() { return SDL_HasMouse(); }
};
} // namespace Terreate::SDL
