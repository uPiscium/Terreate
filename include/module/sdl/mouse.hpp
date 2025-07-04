#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

#include "winhelper.hpp"

namespace Terreate::SDL {
class Mouse {
private:
  PROHIBIT_COPY_AND_ASSIGN(Mouse);

private:
  SDL_Window *mWindow = nullptr;

private:
  friend class Window;
  void setWindow(SDL_Window *window) { mWindow = window; }

public:
  Mouse() = default;
  ~Mouse() = default;

  bool getMouseButton(MouseButton const &button) const;
  pair<float> getCursorPosition() const;

  void setCursorPosition(double xpos, double ypos) const {
    SDL_WarpMouseInWindow(mWindow, (i32)xpos, (i32)ypos);
  }
};
} // namespace Terreate::SDL
