#include "../../../include/module/sdl/exception.hpp"
#include "../../../include/module/sdl/window.hpp"

namespace Terreate::SDL {
Window::Window(u32 const &width, u32 const &height, str const &title) {
  mWindow = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
  mContext = SDL_GL_CreateContext(mWindow);
  if (!mWindow || !mContext) {
    throw Exception::WindowError("Failed to create SDL window or context: " +
                                 std::string(SDL_GetError()));
  }
  SDL_GL_MakeCurrent(mWindow, mContext);

  mProperty.setWindow(mWindow);
  mMouse.setWindow(mWindow);
}

bool Window::isFullScreen() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_FULLSCREEN;
}

bool Window::isWindowed() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return !(SDL_GetWindowFlags(mWindow) & SDL_WINDOW_FULLSCREEN);
}

bool Window::isIconified() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED;
}

bool Window::isMaximized() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MAXIMIZED;
}

bool Window::isVisible() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return !(SDL_GetWindowFlags(mWindow) & SDL_WINDOW_HIDDEN);
}

bool Window::isFocused() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_INPUT_FOCUS;
}

bool Window::isTransparent() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_TRANSPARENT;
}

bool Window::isDecorated() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_BORDERLESS;
}

bool Window::isResizable() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_RESIZABLE;
}

bool Window::isFloating() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_ALWAYS_ON_TOP;
}

void Window::destroy() {
  SDL_DestroyWindow(mWindow);
  SDL_GL_DestroyContext(mContext);
  mWindow = nullptr;
}

void Window::update() const {
  if (mWindow) {
    this->swap();
  }
}

void Window::bind() const {
  if (mWindow) {
    this->setCurrentContext();
    pair<u32> size = mProperty.getFramebufferSize();
    glViewport(0, 0, size.first, size.second);
  }
}
} // namespace Terreate::SDL
