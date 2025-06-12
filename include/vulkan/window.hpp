#pragma once
#include "winevent.hpp"

#include "../common/enum.hpp"
#include "../common/type.hpp"

namespace Terreate::Vulkan {
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

class IWindow {
public:
  virtual ~IWindow() = default;

  virtual WindowProperties const &getProperties() const = 0;
  virtual WindowProperties &getProperties() = 0;
  virtual WindowEvent const &getEvents() const = 0;
  virtual WindowEvent &getEvents() = 0;
  virtual bool getMousebutton(Type::MousebuttonInput const &button) const = 0;
  virtual bool getInputState(Type::InputType const &type) const = 0;

  virtual void setIcon(Icon const &icon) = 0;
  virtual void setCursor(Cursor const &cursor) = 0;
  virtual void setCursor(StandardCursor const &cursor) = 0;

  virtual bool isClosed() const = 0;
  virtual bool isFullScreen() const = 0;
  virtual bool isWindowed() const = 0;
  virtual bool isIconified() const = 0;
  virtual bool isMaximized() const = 0;
  virtual bool isVisible() const = 0;
  virtual bool isFocused() const = 0;
  virtual bool isTransparent() const = 0;
  virtual bool isDecorated() const = 0;
  virtual bool isResizable() const = 0;
  virtual bool isFloating() const = 0;
  virtual bool isAutoIconified() const = 0;
  virtual bool isFocusOnShowed() const = 0;
  virtual bool isPressing(Type::Keyboard const &key) const = 0;
  virtual bool isEntering() const = 0;

  virtual void enableVsync() const = 0;
  virtual void disableVsync() const = 0;
  virtual void iconify() const = 0;
  virtual void maximize() const = 0;
  virtual void show() const = 0;
  virtual void hide() const = 0;
  virtual void focus() const = 0;
  virtual void restore() const = 0;
  virtual void requestAttention() const = 0;
  virtual void close() = 0;
  virtual operator GLFWwindow *() const = 0;
};

class Window : public IWindow {
private:
  PROHIBIT_COPY_AND_ASSIGN(Window);

private:
  VkInstance mInstance = VK_NULL_HANDLE;
  GLFWwindow *mWindow = nullptr;
  WindowProperties mProperties;
  WindowEvent mEvents;

private:
  void destroy();

public:
  Window(VkInstance instance, Type::str const &title,
         Type::pair<Type::i32> const &size, WindowSettings const &settings);
  ~Window() override { this->destroy(); }

  WindowProperties const &getProperties() const override { return mProperties; }
  WindowProperties &getProperties() override { return mProperties; }
  WindowEvent const &getEvents() const override { return mEvents; }
  WindowEvent &getEvents() override { return mEvents; }
  bool getMousebutton(Type::MousebuttonInput const &button) const override {
    return bool(glfwGetMouseButton(mWindow, (Type::u32)button) == GLFW_PRESS);
  }
  bool getInputState(Type::InputType const &type) const override {
    return bool(glfwGetInputMode(mWindow, (Type::u32)type) == GLFW_TRUE);
  }

  void setIcon(Icon const &icon) override;
  void setCursor(Cursor const &cursor) override;
  void setCursor(StandardCursor const &cursor) override;

  bool isClosed() const override {
    return mWindow == nullptr || glfwWindowShouldClose(mWindow);
  }
  bool isFullScreen() const override {
    return bool(glfwGetWindowMonitor(mWindow) != NULL);
  }
  bool isWindowed() const override {
    return bool(glfwGetWindowMonitor(mWindow) == NULL);
  }
  bool isIconified() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_ICONIFIED));
  }
  bool isMaximized() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_MAXIMIZED));
  }
  bool isVisible() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_VISIBLE));
  }
  bool isFocused() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUSED));
  }
  bool isTransparent() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_TRANSPARENT_FRAMEBUFFER));
  }
  bool isDecorated() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_DECORATED));
  }
  bool isResizable() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_RESIZABLE));
  }
  bool isFloating() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_FLOATING));
  }
  bool isAutoIconified() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_AUTO_ICONIFY));
  }
  bool isFocusOnShowed() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUS_ON_SHOW));
  }
  bool isPressing(Type::Keyboard const &key) const override {
    return bool(glfwGetKey(mWindow, (Type::u32)key) == GLFW_PRESS);
  }
  bool isEntering() const override {
    return bool(glfwGetWindowAttrib(mWindow, GLFW_HOVERED));
  }

  void enableVsync() const override { glfwSwapInterval(1); }
  void disableVsync() const override { glfwSwapInterval(0); }
  void iconify() const override { glfwIconifyWindow(mWindow); }
  void maximize() const override { glfwMaximizeWindow(mWindow); }
  void show() const override { glfwShowWindow(mWindow); }
  void hide() const override { glfwHideWindow(mWindow); }
  void focus() const override { glfwFocusWindow(mWindow); }
  void restore() const override { glfwRestoreWindow(mWindow); }
  void requestAttention() const override {
    glfwRequestWindowAttention(mWindow);
  }
  void close() override;

  operator GLFWwindow *() const override { return mWindow; }
};
} // namespace Terreate::Vulkan
