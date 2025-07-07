#pragma once

#include "../common/type.hpp"

namespace Terreate::SDL {
class WindowProperty {
private:
  PROHIBIT_COPY_AND_ASSIGN(WindowProperty);

private:
  SDL_Window *mWindow = nullptr;

private:
  friend class Window;
  void setWindow(SDL_Window *window) { mWindow = window; }

public:
  WindowProperty() = default;
  ~WindowProperty() = default;

  pair<i32> getPosition() const;
  pair<u32> getSize() const;
  str getClipboard() const { return SDL_GetClipboardText(); }
  str getTitle() const { return SDL_GetWindowTitle(mWindow); }
  float getOpacity() const { return (float)SDL_GetWindowOpacity(mWindow); }
  pair<u32> getFramebufferSize() const;
  float getScale() const {
    return SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(mWindow));
  }

  void setPosition(i32 const &xpos, i32 const &ypos) {
    SDL_SetWindowPosition(mWindow, xpos, ypos);
  }
  void setSize(u32 const &width, u32 const &height) {
    SDL_SetWindowSize(mWindow, static_cast<i32>(width),
                      static_cast<i32>(height));
  }
  void setClipboard(str const &text) { SDL_SetClipboardText(text.c_str()); }
  void setTitle(str const &title) {
    SDL_SetWindowTitle(mWindow, title.c_str());
  }
  void setOpacity(float const &opacity) {
    SDL_SetWindowOpacity(mWindow, opacity);
  }
};
} // namespace Terreate::SDL
