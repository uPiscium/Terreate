#pragma once

#include "common/math.hpp"
#include "common/type.hpp"

#include "enum.hpp"

namespace Terreate::Core::SDL {

class Cursor {
private:
  PROHIBIT_COPY_AND_ASSIGN(Cursor);

private:
  SDL_Surface *mCursorImage = nullptr;
  SDL_Cursor *mCursor = nullptr;
  i32 mXHot = 0;
  i32 mYHot = 0;

public:
  Cursor(i32 const &xHot = 0, i32 const &yHot = 0);
  ~Cursor();

  pair<u32> getHotspot() const;

  void setImage(u32 const &width, u32 const &height, ubyte const *pixels);

  operator SDL_Cursor *() const;
  operator bool() const;
};

class SystemCursor {
private:
  PROHIBIT_COPY_AND_ASSIGN(SystemCursor);

private:
  SDL_Cursor *mCursor = nullptr;

public:
  SystemCursor(CursorShape const &shape);
  ~SystemCursor();

  operator SDL_Cursor *() const;
  operator bool() const;
};

class Mouse {
private:
  PROHIBIT_COPY_AND_ASSIGN(Mouse);

private:
  SDL_MouseID mMouseID = 0;

public:
  Mouse(SDL_MouseID const &mouseID);
  ~Mouse() = default;

  str getName() const;
  bool getMouseButton(MouseButton const &button) const;
  vec2 getCursorPosition() const;
  vec2 getGlobalCursorPosition() const;
  vec2 getRelativeCursorPosition() const;

  void setCursor(Cursor const &cursor) const;
  void setCursor(SystemCursor const &cursor) const;
  void setCursorPosition(float const &x, float const &y) const;
  void setCursorPosition(vec2 const &pos) const;
  void setGlobalCursorPosition(float const &x, float const &y) const;
  void setGlobalCursorPosition(vec2 const &pos) const;

  void show() const;
  void hide() const;
  void capture(bool const &capture) const;

public:
  static vec<SDL_MouseID> getMise();
  static bool isCursorVisible();
  static bool hasMouse();
};

} // namespace Terreate::Core::SDL
