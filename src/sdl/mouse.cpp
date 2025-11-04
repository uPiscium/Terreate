#include "sdl/mouse.hpp"

namespace Terreate::SDL {

Cursor::Cursor(i32 const &xHot, i32 const &yHot) : mXHot(xHot), mYHot(yHot) {}

Cursor::~Cursor() {
  SDL_DestroyCursor(mCursor);
  SDL_DestroySurface(mCursorImage);
}

pair<u32> Cursor::getHotspot() const { return {mXHot, mYHot}; }

void Cursor::setImage(u32 const &width, u32 const &height,
                      ubyte const *pixels) {
  mCursorImage = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
  mCursor = SDL_CreateColorCursor(mCursorImage, mXHot, mYHot);
}

Cursor::operator SDL_Cursor *() const { return mCursor; }

Cursor::operator bool() const { return mCursor != nullptr; }

SystemCursor::SystemCursor(CursorShape const &shape) {
  mCursor = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(shape));
}

SystemCursor::~SystemCursor() { SDL_DestroyCursor(mCursor); }

SystemCursor::operator SDL_Cursor *() const { return mCursor; }

SystemCursor::operator bool() const { return mCursor != nullptr; }

Mouse::Mouse(SDL_MouseID const &mouseID) : mMouseID(mouseID) {}

str Mouse::getName() const {
  return mMouseID != 0 ? SDL_GetMouseNameForID(mMouseID)
                       : "SYSTEM SHARED MOUSE";
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

void Mouse::setCursor(Cursor const &cursor) const {
  SDL_SetCursor((SDL_Cursor *)cursor);
}

void Mouse::setCursor(SystemCursor const &cursor) const {
  SDL_SetCursor((SDL_Cursor *)cursor);
}

void Mouse::setCursorPosition(float const &x, float const &y) const {
  SDL_WarpMouseInWindow(SDL_GetMouseFocus(), x, y);
}

void Mouse::setCursorPosition(vec2 const &pos) const {
  SDL_WarpMouseInWindow(SDL_GetMouseFocus(), pos.x, pos.y);
}

void Mouse::setGlobalCursorPosition(float const &x, float const &y) const {
  SDL_WarpMouseGlobal(x, y);
}

void Mouse::setGlobalCursorPosition(vec2 const &pos) const {
  SDL_WarpMouseGlobal(pos.x, pos.y);
}

void Mouse::show() const { SDL_ShowCursor(); }

void Mouse::hide() const { SDL_HideCursor(); }

void Mouse::capture(bool const &capture) const { SDL_CaptureMouse(capture); }

vec<SDL_MouseID> Mouse::getMise() {
  i32 numMice;
  SDL_MouseID *ids = SDL_GetMice(&numMice);
  vec<SDL_MouseID> mice(ids, ids + numMice);
  SDL_free(ids);
  return mice;
}

bool Mouse::isCursorVisible() { return SDL_CursorVisible(); }

bool Mouse::hasMouse() { return SDL_HasMouse(); }

} // namespace Terreate::SDL
