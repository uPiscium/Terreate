#include "sdl/exception.hpp"
#include "sdl/window.hpp"

namespace Terreate::SDL {

u64 WindowSettings::toFlags() const {
  u64 flags = 0;
  if (fullscreen)
    flags |= SDL_WINDOW_FULLSCREEN;
  if (occluded)
    flags |= SDL_WINDOW_OCCLUDED;
  if (hidden)
    flags |= SDL_WINDOW_HIDDEN;
  if (borderless)
    flags |= SDL_WINDOW_BORDERLESS;
  if (resizable)
    flags |= SDL_WINDOW_RESIZABLE;
  if (minimized)
    flags |= SDL_WINDOW_MINIMIZED;
  if (maximized)
    flags |= SDL_WINDOW_MAXIMIZED;
  if (mouseGrabbed)
    flags |= SDL_WINDOW_MOUSE_GRABBED;
  if (inputFocus)
    flags |= SDL_WINDOW_INPUT_FOCUS;
  if (mouseFocus)
    flags |= SDL_WINDOW_MOUSE_FOCUS;
  if (highDPI)
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
  if (mouseCapture)
    flags |= SDL_WINDOW_MOUSE_CAPTURE;
  if (relativeMouseMode)
    flags |= SDL_WINDOW_MOUSE_RELATIVE_MODE;
  if (alwaysOnTop)
    flags |= SDL_WINDOW_ALWAYS_ON_TOP;
  if (utility)
    flags |= SDL_WINDOW_UTILITY;
  if (toolTip)
    flags |= SDL_WINDOW_TOOLTIP;
  if (popupMenu)
    flags |= SDL_WINDOW_POPUP_MENU;
  if (keyboardGrabbed)
    flags |= SDL_WINDOW_KEYBOARD_GRABBED;
  if (transparent)
    flags |= SDL_WINDOW_TRANSPARENT;
  if (notFocusable)
    flags |= SDL_WINDOW_NOT_FOCUSABLE;
  return flags;
}

Window::Window(shared<Vulkan::Instance> instance, u32 const &width,
               u32 const &height, str const &title, shared<Mouse> mouse,
               u64 const &flags)
    : mInstance(instance) {
  this->mouse = mouse;
  mWindow =
      SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_VULKAN | flags);
  SDL_Vulkan_CreateSurface(mWindow, *instance, nullptr, &mSurface);
  if (!mWindow) {
    throw WindowException("Failed to create SDL window or context: " +
                          str(SDL_GetError()));
  }
}

Window::~Window() {
  vkDestroySurfaceKHR(*mInstance, mSurface, nullptr);
  this->destroy();
}

WindowID Window::getID() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowID(mWindow);
}

pair<i32> Window::getPosition() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  i32 x, y;
  SDL_GetWindowPosition(mWindow, &x, &y);
  return {x, y};
}

pair<i32> Window::getSize() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  i32 width, height;
  SDL_GetWindowSize(mWindow, &width, &height);
  return {width, height};
}

str Window::getClipboard() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  char const *text = SDL_GetClipboardText();
  if (!text) {
    return "";
  }
  return str(text);
}

str Window::getTitle() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return str(SDL_GetWindowTitle(mWindow));
}

float Window::getOpacity() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowOpacity(mWindow);
}

pair<u32> Window::getFramebufferSize() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  i32 width, height;
  SDL_GetWindowSizeInPixels(mWindow, &width, &height);
  return {static_cast<u32>(width), static_cast<u32>(height)};
}

pair<vec2> Window::getMouseRect() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
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

void Window::setPosition(pair<i32> const &position) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  SDL_SetWindowPosition(mWindow, position.first, position.second);
}

void Window::setSize(pair<i32> const &size) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  SDL_SetWindowSize(mWindow, size.first, size.second);
}

void Window::setClipboard(str const &text) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  SDL_SetClipboardText(text.c_str());
}

void Window::setTitle(str const &title) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  SDL_SetWindowTitle(mWindow, title.c_str());
}

void Window::setOpacity(float const &opacity) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  SDL_SetWindowOpacity(mWindow, opacity);
}

void Window::setIcon(Icon const &icon) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  SDL_SetWindowIcon(mWindow, (SDL_Surface *)icon);
}

void Window::setMouseRect(pair<vec2> const &rect) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  SDL_Rect sdlRect = {
      static_cast<i32>(rect.first.x), static_cast<i32>(rect.first.y),
      static_cast<i32>(rect.second.x), static_cast<i32>(rect.second.y)};
  SDL_SetWindowMouseRect(mWindow, &sdlRect);
}

bool Window::isClosed() const { return mWindow == nullptr; }

bool Window::isFullscreen() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_FULLSCREEN;
}

bool Window::isOccluded() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_OCCLUDED;
}

bool Window::isHidden() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_HIDDEN;
}

bool Window::isBorderless() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_BORDERLESS;
}

bool Window::isResizable() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_RESIZABLE;
}

bool Window::isMinimized() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED;
}

bool Window::isMaximized() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MAXIMIZED;
}

bool Window::isMouseGrabbed() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MOUSE_GRABBED;
}

bool Window::isInputFocused() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_INPUT_FOCUS;
}

bool Window::isMouseFocused() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MOUSE_FOCUS;
}

bool Window::isHighDPI() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_HIGH_PIXEL_DENSITY;
}

bool Window::isMouseCaptured() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MOUSE_CAPTURE;
}

bool Window::isRelativeMouseMode() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowRelativeMouseMode(mWindow);
}

bool Window::isAlwaysOnTop() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_ALWAYS_ON_TOP;
}

bool Window::isUtility() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_UTILITY;
}

bool Window::isToolTip() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_TOOLTIP;
}

bool Window::isPopupMenu() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_POPUP_MENU;
}

bool Window::isKeyboardGrabbed() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_KEYBOARD_GRABBED;
}

bool Window::isTransparent() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_TRANSPARENT;
}

bool Window::isNotFocusable() const {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  return SDL_GetWindowFlags(mWindow) & SDL_WINDOW_NOT_FOCUSABLE;
}

void Window::fullscreen(bool const &fullscreen) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_SetWindowFullscreen(mWindow, fullscreen);
}

void Window::visible(bool const &visible) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  if (visible) {
    SDL_ShowWindow(mWindow);
  } else {
    SDL_HideWindow(mWindow);
  }
}

void Window::borderless(bool const &borderless) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_SetWindowBordered(mWindow, !borderless);
}

void Window::resizable(bool const &resizeable) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_SetWindowResizable(mWindow, resizeable);
}

void Window::mouseGrabbed(bool const &grabbed) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_SetWindowMouseGrab(mWindow, grabbed);
}

void Window::mouseCapture(bool const &mouseCapture) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_CaptureMouse(mouseCapture);
}

void Window::relativeMouseMode(bool const &relative) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_SetWindowRelativeMouseMode(mWindow, relative);
}

void Window::alwaysOnTop(bool const &alwaysOnTop) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_SetWindowAlwaysOnTop(mWindow, alwaysOnTop);
}

void Window::keyboardGrabbed(bool const &grabbed) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_SetWindowKeyboardGrab(mWindow, grabbed);
}

void Window::minimize() {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_MinimizeWindow(mWindow);
}

void Window::maximize() {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_MaximizeWindow(mWindow);
}

void Window::requestInputFocus() {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  SDL_RaiseWindow(mWindow);
}

void Window::vsync(bool const &vsync) {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }

  if (vsync) {
    SDL_SetWindowSurfaceVSync(mWindow, SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE);
  } else {
    SDL_SetWindowSurfaceVSync(mWindow, SDL_WINDOW_SURFACE_VSYNC_DISABLED);
  }
}

void Window::destroy() {
  SDL_DestroyWindow(mWindow);
  mWindow = nullptr;
}

void Window::close() {
  if (!mWindow) {
    throw WindowException("Window is not available.");
  }
  this->destroy();
}

Window::operator bool() const { return !this->isClosed(); }

shared<Window> Window::create(shared<Vulkan::Instance> instance,
                              u32 const &width, u32 const &height,
                              str const &title, shared<Mouse> mouse,
                              WindowSettings const &settings) {
  u64 flags = settings.toFlags();
  Window *window = new Window(instance, width, height, title, mouse, flags);
  return shared<Window>(window);
}

} // namespace Terreate::SDL
