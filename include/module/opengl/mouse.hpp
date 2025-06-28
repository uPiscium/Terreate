#pragma once

#include "../common/type.hpp"

#include "winhelper.hpp"

namespace Terreate::OpenGL {
namespace Callbacks {
void mousebuttonCallbackWrapper(GLFWwindow *window, i32 button, i32 action,
                                i32 mods);
void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos);
void cursorEnterCallbackWrapper(GLFWwindow *window, i32 entered);
void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset);
} // namespace Callbacks

typedef Event<Window *, u32 const &, u32 const &, Modifier const &>
    MousebuttonEvent;
typedef Event<Window *, double const &, double const &> CursorPositionEvent;
typedef Event<Window *, bool const &> CursorEnterEvent;
typedef Event<Window *, double const &, double const &> ScrollEvent;

class Mouse {
private:
  PROHIBIT_COPY_AND_ASSIGN(Mouse);

private:
  GLFWwindow *mWindow = nullptr;

  pair<double> mScrollOffset = {0.0, 0.0};

private:
  friend void Callbacks::scrollCallbackWrapper(GLFWwindow *, double, double);
  friend class Window;

  void setWindow(GLFWwindow *window);
  void setScrollOffset(double xoffset, double yoffset) {
    mScrollOffset = {xoffset, yoffset};
  }

public:
  MousebuttonEvent onMousebuttonInput;
  CursorPositionEvent onCursorPositionChange;
  CursorEnterEvent onCursorEnter;
  ScrollEvent onScroll;

public:
  Mouse() = default;
  ~Mouse() = default;

  bool getMousebutton(MousebuttonInput const &button) const {
    return bool(glfwGetMouseButton(mWindow, (u32)button) == GLFW_PRESS);
  }
  pair<double> getCursorPosition() const {
    pair<double> position;
    glfwGetCursorPos(mWindow, &position.first, &position.second);
    return position;
  }
  pair<double> getScrollOffset() const { return mScrollOffset; }

  void setCursorPosition(double xpos, double ypos) const {
    glfwSetCursorPos(mWindow, xpos, ypos);
  }
};
} // namespace Terreate::OpenGL
