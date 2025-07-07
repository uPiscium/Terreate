#include "../../../include/module/sdl/exception.hpp"
#include "../../../include/module/sdl/window.hpp"

namespace Terreate::SDL {
Window::Window(u32 const &width, u32 const &height, str const &title,
               shared<Mouse> mouse)
    : mMouse(mouse) {
  mWindow = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
  mContext = SDL_GL_CreateContext(mWindow);
  if (!mWindow || !mContext) {
    throw Exception::WindowError("Failed to create SDL window or context: " +
                                 std::string(SDL_GetError()));
  }
  SDL_GL_MakeCurrent(mWindow, mContext);

  mProperty.setWindow(mWindow);
}

pair<vec2> Window::getMouseRect() const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  SDL_Rect const *rect = SDL_GetWindowMouseRect(mWindow);
  if (!rect) {
    return {{0, 0}, {0, 0}};
  }
  if (rect->w <= 0 || rect->h <= 0) {
    return {{0, 0}, {0, 0}};
  }
  return {{(float)rect->x, (float)rect->y}, {(float)rect->w, (float)rect->h}};
}

void Window::setMouseRect(pair<vec2> const &rect) const {
  if (!mWindow) {
    throw Exception::WindowError("Window is not available.");
  }
  SDL_Rect sdlRect = {
      static_cast<i32>(rect.first.x), static_cast<i32>(rect.first.y),
      static_cast<i32>(rect.second.x), static_cast<i32>(rect.second.y)};
  SDL_SetWindowMouseRect(mWindow, &sdlRect);
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
