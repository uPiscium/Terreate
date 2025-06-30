#include "../../../include/module/opengl/window.hpp"

namespace Terreate::OpenGL {
Window::Window(u32 const &width, u32 const &height, str const &title,
               WindowSettings const &settings, Window *shared) {
  glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
  glfwWindowHint(GLFW_VISIBLE, settings.visible);
  glfwWindowHint(GLFW_DECORATED, settings.decorated);
  glfwWindowHint(GLFW_FOCUSED, settings.focused);
  glfwWindowHint(GLFW_AUTO_ICONIFY, settings.autoIconify);
  glfwWindowHint(GLFW_FLOATING, settings.floating);
  glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);
  glfwWindowHint(GLFW_CENTER_CURSOR, settings.centerCursor);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, settings.transparentFramebuffer);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, settings.focusOnShow);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, settings.scaleToMonitor);

  GLFWwindow *sharedWindow = shared ? shared->mWindow : nullptr;
  mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, sharedWindow);
  glfwSetWindowUserPointer(mWindow, this);
  glfwMakeContextCurrent(mWindow);

  mProperty.setWindow(mWindow);
  mEvent.setWindow(mWindow);
  mMouse.setWindow(mWindow);
}

void Window::destroy() {
  glfwDestroyWindow(mWindow);
  mWindow = nullptr;
}

void Window::update() const {
  if (mWindow) {
    this->swap();
  }
}

void Window::bind() const {
  if (mWindow) {
    glfwMakeContextCurrent(mWindow);
  }
  pair<u32> size = mProperty.getFramebufferSize();
  glViewport(0, 0, size.first, size.second);
}
} // namespace Terreate::OpenGL
