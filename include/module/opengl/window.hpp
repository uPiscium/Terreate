#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

#include "cursor.hpp"
#include "icon.hpp"
#include "mouse.hpp"
#include "winhelper.hpp"

namespace Terreate::OpenGL {
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
};

class Window {
private:
  GLFWwindow *mWindow = nullptr;
  WindowProperty mProperty;
  WindowEvent mEvent;
  Mouse mMouse;

private:
  Window(Window const &) = delete;
  Window &operator=(Window const &) = delete;

public:
  Window(u32 const &width, u32 const &height, str const &title,
         WindowSettings const &settings, Window *shared = nullptr);
  ~Window() { this->destroy(); }

  WindowProperty const &getProperty() const { return mProperty; }
  WindowProperty &getProperty() { return mProperty; }
  WindowEvent &getEvent() { return mEvent; }
  Mouse const &getMouse() const { return mMouse; }
  Mouse &getMouse() { return mMouse; }

  void setCurrentContext() const { glfwMakeContextCurrent(mWindow); }
  void setIcon(Icon const &icon) {
    glfwSetWindowIcon(mWindow, icon.getSize(), (GLFWimage const *)icon);
  }
  void setCursor(Cursor const &cursor) {
    glfwSetCursor(mWindow, (GLFWcursor *)cursor);
  }
  void setCursor(StandardCursor const &cursor) {
    glfwSetCursor(mWindow, (GLFWcursor *)cursor);
  }
  void setDefaultCursor() { glfwSetCursor(mWindow, nullptr); }
  void setInputTypeState(InputType const &type, bool const &state) {
    glfwSetInputMode(mWindow, (int)type, state);
  }
  void setViewPort(u32 const &x0, u32 const &y0, u32 const &width,
                   u32 const &height) {
    glViewport(x0, y0, width, height);
  }
  void setViewPort(u32 const &width, u32 const &height) {
    glViewport(0, 0, width, height);
  }

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
  bool isPressing(Keyboard const &key) const {
    return bool(glfwGetKey(mWindow, (u32)key) == GLFW_PRESS);
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
  void fill(float const &r, float const &g, float const &b) const {
    glClearColor(r, g, b, 0.0f);
  }
  void clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }
  void clearColor() const { glClear(GL_COLOR_BUFFER_BIT); }
  void clearDepth() const { glClear(GL_DEPTH_BUFFER_BIT); }
  void clearStencil() const { glClear(GL_STENCIL_BUFFER_BIT); }
  void swap() const { glfwSwapBuffers(mWindow); }
  void pollEvents() const { glfwPollEvents(); }
  void bind() const;
  void unbind() const { glfwMakeContextCurrent(nullptr); }

  operator bool() const { return !this->isClosed(); }
};
} // namespace Terreate::OpenGL
