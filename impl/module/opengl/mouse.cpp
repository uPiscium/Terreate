#include "../../../include/module/opengl/mouse.hpp"
#include "../../../include/module/opengl/window.hpp"

namespace Terreate::OpenGL {
namespace Callbacks {
void mousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
                                int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getMouse().onMousebuttonInput.publish(ptr, button, action,
                                             Modifier(mods));
}

void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getMouse().setCursorPosition(xpos, ypos);
  ptr->getMouse().onCursorPositionChange.publish(ptr, xpos, ypos);
}

void cursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getMouse().onCursorEnter.publish(ptr, (bool)entered);
}

void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->getMouse().setScrollOffset(xoffset, yoffset);
  ptr->getMouse().onScroll.publish(ptr, xoffset, yoffset);
}
} // namespace Callbacks

void Mouse::setWindow(GLFWwindow *window) {
  mWindow = window;
  glfwSetMouseButtonCallback(mWindow, Callbacks::mousebuttonCallbackWrapper);
  glfwSetCursorPosCallback(mWindow, Callbacks::cursorPositionCallbackWrapper);
  glfwSetCursorEnterCallback(mWindow, Callbacks::cursorEnterCallbackWrapper);
  glfwSetScrollCallback(mWindow, Callbacks::scrollCallbackWrapper);
}
} // namespace Terreate::OpenGL
