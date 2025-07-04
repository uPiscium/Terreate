#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

#include "cursor.hpp"
#include "icon.hpp"
#include "mouse.hpp"
#include "winhelper.hpp"

namespace Terreate::SDL {
class Window {
private:
  PROHIBIT_COPY_AND_ASSIGN(Window);

private:
  SDL_Window *mWindow = nullptr;
  SDL_GLContext mContext = nullptr;
  WindowProperty mProperty;
  Mouse mMouse;

public:
  Window(u32 const &width, u32 const &height, str const &title);
  ~Window() { this->destroy(); }

  SDL_WindowID getId() const { return SDL_GetWindowID(mWindow); }
  WindowProperty const &getProperty() const { return mProperty; }
  WindowProperty &getProperty() { return mProperty; }
  Mouse const &getMouse() const { return mMouse; }
  Mouse &getMouse() { return mMouse; }

  void setCurrentContext() const { SDL_GL_MakeCurrent(mWindow, mContext); }
  void setIcon(Icon const &icon) {
    SDL_SetWindowIcon(mWindow, (SDL_Surface *)icon);
  }
  void setCursor(Cursor const &cursor) { SDL_SetCursor((SDL_Cursor *)cursor); }
  void setCursor(SystemCursor const &cursor) {
    SDL_SetCursor((SDL_Cursor *)cursor);
  }
  void setViewPort(u32 const &x0, u32 const &y0, u32 const &width,
                   u32 const &height) {
    glViewport(x0, y0, width, height);
  }
  void setViewPort(u32 const &width, u32 const &height) {
    glViewport(0, 0, width, height);
  }
  void setResizable(bool const &resizable) const {
    bool s = SDL_SetWindowResizable(mWindow, resizable);
    if (!s) {
      throw std::runtime_error("Failed to set window resizable: " +
                               std::string(SDL_GetError()));
    }
  }
  void setBordered(bool const &bordered) const {
    SDL_SetWindowBordered(mWindow, bordered);
  }
  void setFloating(bool const &floating) const {
    SDL_SetWindowAlwaysOnTop(mWindow, floating);
  }

  bool isClosed() const { return mWindow == nullptr; }
  bool isFullScreen() const;
  bool isWindowed() const;
  bool isIconified() const;
  bool isMaximized() const;
  bool isVisible() const;
  bool isFocused() const;
  bool isTransparent() const;
  bool isDecorated() const;
  bool isResizable() const;
  bool isFloating() const;

  void enableVsync() const {
    SDL_SetWindowSurfaceVSync(mWindow, SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE);
  }
  void disableVsync() const {
    SDL_SetWindowSurfaceVSync(mWindow, SDL_WINDOW_SURFACE_VSYNC_DISABLED);
  }
  void destroy();
  void close() { this->destroy(); }
  void iconify() const { SDL_MinimizeWindow(mWindow); }
  void maximize() const { SDL_MaximizeWindow(mWindow); }
  void show() const { SDL_ShowWindow(mWindow); }
  void hide() const { SDL_HideWindow(mWindow); }
  void focus() const { SDL_RaiseWindow(mWindow); }
  void restore() const { SDL_RestoreWindow(mWindow); }
  void fill(float const &r, float const &g, float const &b) const {
    glClearColor(r, g, b, 0.0f);
  }
  void clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }
  void clearColor() const { glClear(GL_COLOR_BUFFER_BIT); }
  void clearDepth() const { glClear(GL_DEPTH_BUFFER_BIT); }
  void clearStencil() const { glClear(GL_STENCIL_BUFFER_BIT); }
  void swap() const { SDL_GL_SwapWindow(mWindow); }
  void update() const;
  void bind() const;
  void unbind() const { SDL_GL_MakeCurrent(nullptr, mContext); }

  operator bool() const { return !this->isClosed(); }
};
} // namespace Terreate::SDL
