#pragma once
#include "device.hpp"
#include "swapchain.hpp"
#include "vkobj.hpp"
#include "winevent.hpp"

#include "../common/enum.hpp"
#include "../common/type.hpp"

namespace Terreate::Core {
struct WindowSettings {
public:
  bool resizable = true;
  bool visible = true;
  bool decorated = true;
  bool focused = true;
  bool autoIconify = true;
  bool floating = false;
  bool maximized = false;
  bool centerCursor = true;
  bool transparentFramebuffer = false;
  bool focusOnShow = true;
  bool scaleToMonitor = false;

public:
  void apply() const;
};

class Icon final {
private:
  Type::vec<GLFWimage> mImages;
  Type::vec<Type::u8 *> mPointers;

private:
  Icon(Icon const &) = delete;
  Icon &operator=(Icon const &) = delete;

public:
  Icon();
  ~Icon();

  Type::u32 getImageCount() const;

  void addImage(Type::u32 const &width, Type::u32 const &height,
                Type::u8 const *pixels);

  operator GLFWimage const *() const;
  operator bool() const;
};

class Cursor final {
private:
  GLFWcursor *mCursor = nullptr;
  GLFWimage mImage = GLFWimage();
  Type::u8 *mPixels = nullptr;
  int mXHot = 0;
  int mYHot = 0;

private:
  Cursor(Cursor const &) = delete;
  Cursor &operator=(Cursor const &) = delete;

public:
  Cursor(Type::i32 const &xHot = 0, Type::i32 const &yHot = 0);
  ~Cursor();

  void setImage(Type::u32 const &width, Type::u32 const &height,
                Type::u8 const *pixels);

  operator GLFWcursor *() const;
  operator bool() const;
};

class StandardCursor {
private:
  GLFWcursor *mCursor = nullptr;

private:
  StandardCursor(StandardCursor const &) = delete;
  StandardCursor &operator=(StandardCursor const &) = delete;

public:
  StandardCursor() = default;
  StandardCursor(Type::CursorShape const &shape);
  ~StandardCursor();

  operator GLFWcursor *() const;
  operator bool() const;
};

class Window {
private:
  PROHIBIT_COPY_AND_ASSIGN(Window);

private:
  VkInstance mInstance = VK_NULL_HANDLE;

  GLFWwindow *mWindow = nullptr;

public:
  WindowProperties properties;
  WindowEvent events;

public:
  Window(VkInstance instance, Type::str const &title,
         Type::pair<Type::i32> const &size, WindowSettings const &settings);
  ~Window() { this->destroy(); }

  bool getMousebutton(Type::MousebuttonInput const &button) const;
  bool getInputState(Type::InputType const &type) const;

  void setIcon(Icon const &icon);
  void setCursor(Cursor const &cursor);
  void setCursor(StandardCursor const &cursor);

  bool isClosed() const {
    return mWindow == nullptr || glfwWindowShouldClose(mWindow);
  }
  bool isFullScreen() const {
    return bool(glfwGetWindowMonitor(mWindow) != NULL);
  }
  bool isWindowed() const {
    return bool(glfwGetWindowMonitor(mWindow) == NULL);
  }
  bool isIconified() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_ICONIFIED));
  }
  bool isMaximized() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_MAXIMIZED));
  }
  bool isVisible() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_VISIBLE));
  }
  bool isFocused() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUSED));
  }
  bool isTransparent() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_TRANSPARENT_FRAMEBUFFER));
  }
  bool isDecorated() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_DECORATED));
  }
  bool isResizable() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_RESIZABLE));
  }
  bool isFloating() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_FLOATING));
  }
  bool isAutoIconified() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_AUTO_ICONIFY));
  }
  bool isFocusOnShowed() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUS_ON_SHOW));
  }
  bool isPressing(Type::Keyboard const &key) const {
    return bool(glfwGetKey(mWindow, (Type::u32)key) == GLFW_PRESS);
  }
  bool isEntering() const {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_HOVERED));
  }

  void enableVsync() const { glfwSwapInterval(1); }
  void disableVsync() const { glfwSwapInterval(0); }
  void destroy();
  void close() const { glfwSetWindowShouldClose(mWindow, GLFW_TRUE); }
  void iconify() const { glfwIconifyWindow(mWindow); }
  void maximize() const { glfwMaximizeWindow(mWindow); }
  void show() const { glfwShowWindow(mWindow); }
  void hide() const { glfwHideWindow(mWindow); }
  void focus() const { glfwFocusWindow(mWindow); }
  void restore() const { glfwRestoreWindow(mWindow); }
  void requestAttention() const { glfwRequestWindowAttention(mWindow); }

  operator GLFWwindow *() const { return mWindow; }
};
} // namespace Terreate::Core
