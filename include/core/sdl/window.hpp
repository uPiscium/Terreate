#pragma once

#include "common/type.hpp"

#include "core/vulkan/instance.hpp"

#include "icon.hpp"
#include "mouse.hpp"

namespace Terreate::SDL {

class WindowSettings {
public:
  bool fullscreen = false;
  bool occluded = false;
  bool hidden = false;
  bool borderless = false;
  bool resizable = true;
  bool minimized = false;
  bool maximized = false;
  bool mouseGrabbed = false;
  bool inputFocus = true;
  bool mouseFocus = true;
  bool highDPI = false;
  bool mouseCapture = false;
  bool relativeMouseMode = false;
  bool alwaysOnTop = false;
  bool utility = false;
  bool toolTip = false;
  bool popupMenu = false;
  bool keyboardGrabbed = false;
  bool transparent = false;
  bool notFocusable = false;

public:
  u64 toFlags() const;
};

class Window {
private:
  PROHIBIT_COPY_AND_ASSIGN(Window);

private:
  shared<Vulkan::Instance> mInstance = nullptr;
  SDL_Window *mWindow = nullptr;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;

public:
  shared<Mouse> mouse = nullptr;

private:
  Window(shared<Vulkan::Instance> instance, u32 const &width, u32 const &height,
         str const &title, shared<Mouse> mouse, u64 const &flags);

public:
  ~Window();

  WindowID getID() const;
  pair<i32> getPosition() const;
  pair<i32> getSize() const;
  str getClipboard() const;
  str getTitle() const;
  float getOpacity() const;
  pair<u32> getFramebufferSize() const;
  pair<vec2> getMouseRect() const;

  void setPosition(pair<i32> const &position);
  void setSize(pair<i32> const &size);
  void setClipboard(str const &text);
  void setTitle(str const &title);
  void setOpacity(float const &opacity);
  void setIcon(Icon const &icon);
  void setMouseRect(pair<vec2> const &rect);

  bool isClosed() const;
  bool isFullscreen() const;
  bool isOccluded() const;
  bool isHidden() const;
  bool isBorderless() const;
  bool isResizable() const;
  bool isMinimized() const;
  bool isMaximized() const;
  bool isMouseGrabbed() const;
  bool isInputFocused() const;
  bool isMouseFocused() const;
  bool isHighDPI() const;
  bool isMouseCaptured() const;
  bool isRelativeMouseMode() const;
  bool isAlwaysOnTop() const;
  bool isUtility() const;
  bool isToolTip() const;
  bool isPopupMenu() const;
  bool isKeyboardGrabbed() const;
  bool isTransparent() const;
  bool isNotFocusable() const;

  void fullscreen(bool const &fullscreen);
  void visible(bool const &visible);
  void borderless(bool const &borderless);
  void resizable(bool const &resizable);
  void mouseGrabbed(bool const &grabbed);
  void mouseCapture(bool const &mouseCapture);
  void relativeMouseMode(bool const &relative);
  void alwaysOnTop(bool const &alwaysOnTop);
  void keyboardGrabbed(bool const &grabbed);

  void minimize();
  void maximize();
  void requestInputFocus();

  void vsync(bool const &vsync);
  void destroy();
  void close();

  operator bool() const;
  operator VkSurfaceKHR const &() const;

public:
  static shared<Window>
  create(shared<Vulkan::Instance> instance, u32 const &width, u32 const &height,
         str const &title, shared<Mouse> mouse = nullptr,
         WindowSettings const &settings = WindowSettings());
};

} // namespace Terreate::SDL
