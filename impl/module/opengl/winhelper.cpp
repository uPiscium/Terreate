#include "../../../include/module/opengl/exception.hpp"
#include "../../../include/module/opengl/window.hpp"
#include "../../../include/module/opengl/winhelper.hpp"

namespace Terreate::OpenGL {
namespace Callbacks {
void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getProperty().setPosition(xpos, ypos);
  ptr->getEvent().onPositionChange.publish(ptr, xpos, ypos);
}

void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getProperty().setSize(width, height);
  ptr->getEvent().onSizeChange.publish(ptr, width, height);
}

void windowCloseCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getEvent().onClose.publish(ptr);
}

void windowRefreshCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getEvent().onRefresh.publish(ptr);
}

void windowFocusCallbackWrapper(GLFWwindow *window, int focused) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getEvent().onFocus.publish(ptr, (bool)focused);
}

void windowIconifyCallbackWrapper(GLFWwindow *window, int iconified) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getEvent().onIconify.publish(ptr, (bool)iconified);
}

void windowMaximizeCallbackWrapper(GLFWwindow *window, int maximized) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getEvent().onMaximize.publish(ptr, (bool)maximized);
}

void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                          int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getEvent().onFramebufferSizeChange.publish(ptr, width, height);
}

void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getEvent().onContentScaleChange.publish(ptr, xscale, yscale);
}

void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Key wrappedKey = Key(key, scancode, action, mods);
  ptr->getProperty().addKey(wrappedKey);
  ptr->getEvent().onKeyInput.publish(ptr, wrappedKey);
}

void charCallbackWrapper(GLFWwindow *window, u32 codepoint) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getProperty().addCodePoint(codepoint);
  ptr->getEvent().onCharInput.publish(ptr, codepoint);
}

void dropCallbackWrapper(GLFWwindow *window, int count, char const **paths) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  vec<str> droppedFiles(paths, paths + count);
  ptr->getProperty().addDroppedFiles(droppedFiles);
  ptr->getEvent().onFileDrop.publish(ptr, droppedFiles);
}
} // namespace Callbacks

pair<i32> WindowProperty::getPosition() const {
  i32 xpos, ypos;
  glfwGetWindowPos(mWindow, &xpos, &ypos);
  return pair<i32>(xpos, ypos);
}

pair<u32> WindowProperty::getSize() const {
  i32 width, height;
  glfwGetWindowSize(mWindow, &width, &height);
  return pair<u32>(static_cast<u32>(width), static_cast<u32>(height));
}

void WindowProperty::clearAllInputs() {
  mKeys.clear();
  mCodePoints.clear();
  mDroppedFiles.clear();
}

pair<u32> WindowProperty::getFramebufferSize() const {
  i32 width, height;
  glfwGetFramebufferSize(mWindow, &width, &height);
  return pair<u32>{static_cast<u32>(width), static_cast<u32>(height)};
}

pair<float> WindowProperty::getContentScale() const {
  float xscale, yscale;
  glfwGetWindowContentScale(mWindow, &xscale, &yscale);
  return pair<float>{xscale, yscale};
}

pair<i32> WindowProperty::getMonitorPosition() const {
  GLFWmonitor *monitor = glfwGetWindowMonitor(mWindow);
  if (monitor == nullptr) {
    return pair<i32>{0, 0};
  }
  int xpos, ypos;
  glfwGetMonitorPos(monitor, &xpos, &ypos);
  return pair<i32>{xpos, ypos};
}

pair<u32> WindowProperty::getMonitorSize() const {
  GLFWmonitor *monitor = glfwGetWindowMonitor(mWindow);
  if (monitor == nullptr) {
    return pair<u32>{0, 0};
  }
  int width, height;
  glfwGetMonitorPhysicalSize(monitor, &width, &height);
  return pair<u32>{static_cast<u32>(width), static_cast<u32>(height)};
}

pair<u32> WindowProperty::getMonitorPhysicalSize() const {
  GLFWmonitor *monitor = glfwGetWindowMonitor(mWindow);
  if (monitor == nullptr) {
    return pair<u32>{0, 0};
  }
  int width, height;
  glfwGetMonitorPhysicalSize(monitor, &width, &height);
  return pair<u32>{static_cast<u32>(width), static_cast<u32>(height)};
}

u32 WindowProperty::getMonitorRefreshRate() const {
  GLFWmonitor *monitor = glfwGetWindowMonitor(mWindow);
  if (monitor == nullptr) {
    return 0;
  }
  int refreshRate = glfwGetVideoMode(monitor)->refreshRate;
  return static_cast<u32>(refreshRate);
}

str WindowProperty::getMonitorName() const {
  GLFWmonitor *monitor = glfwGetWindowMonitor(mWindow);
  if (monitor == nullptr) {
    return "";
  }
  const char *name = glfwGetMonitorName(monitor);
  return name ? str(name) : "";
}

void WindowEvent::setWindow(GLFWwindow *window) {
  if (window == nullptr) {
    throw Exception::WindowError("Window pointer cannot be null.");
  }
  mWindow = window;

  glfwSetWindowPosCallback(mWindow, Callbacks::windowPositionCallbackWrapper);
  glfwSetWindowSizeCallback(mWindow, Callbacks::windowSizeCallbackWrapper);
  glfwSetWindowCloseCallback(mWindow, Callbacks::windowCloseCallbackWrapper);
  glfwSetWindowRefreshCallback(mWindow,
                               Callbacks::windowRefreshCallbackWrapper);
  glfwSetWindowFocusCallback(mWindow, Callbacks::windowFocusCallbackWrapper);
  glfwSetWindowIconifyCallback(mWindow,
                               Callbacks::windowIconifyCallbackWrapper);
  glfwSetWindowMaximizeCallback(mWindow,
                                Callbacks::windowMaximizeCallbackWrapper);
  glfwSetFramebufferSizeCallback(
      mWindow, Callbacks::windowFramebufferSizeCallbackWrapper);
  glfwSetWindowContentScaleCallback(
      mWindow, Callbacks::windowContentScaleCallbackWrapper);
  glfwSetKeyCallback(mWindow, Callbacks::keyCallbackWrapper);
  glfwSetCharCallback(mWindow, Callbacks::charCallbackWrapper);
  glfwSetDropCallback(mWindow, Callbacks::dropCallbackWrapper);
}

} // namespace Terreate::OpenGL
