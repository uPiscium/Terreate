#pragma once

#include "../common/enum.hpp"
#include "../common/event.hpp"
#include "../common/type.hpp"

namespace Terreate::OpenGL {
struct Modifier {
public:
  bool shift = false;
  bool control = false;
  bool alt = false;
  bool super = false;
  bool capsLock = false;
  bool numLock = false;

public:
  Modifier(i32 mods_)
      : shift(mods_ & GLFW_MOD_SHIFT), control(mods_ & GLFW_MOD_CONTROL),
        alt(mods_ & GLFW_MOD_ALT), super(mods_ & GLFW_MOD_SUPER),
        capsLock(mods_ & GLFW_MOD_CAPS_LOCK),
        numLock(mods_ & GLFW_MOD_NUM_LOCK) {}
};

struct Key {
public:
  Keyboard key = Keyboard::K_LAST;
  i32 scancode = 0;
  bool pressed = 0;
  Modifier mods = 0;

public:
  Key(i32 key_, i32 scancode_, i32 action_, i32 mods_)
      : key((Keyboard)key_), scancode(scancode_), pressed((bool)action_),
        mods(mods_) {}
};

namespace Callbacks {
void windowPositionCallbackWrapper(GLFWwindow *window, i32 xpos, i32 ypos);
void windowSizeCallbackWrapper(GLFWwindow *window, i32 width, i32 height);
void windowCloseCallbackWrapper(GLFWwindow *window);
void windowRefreshCallbackWrapper(GLFWwindow *window);
void windowFocusCallbackWrapper(GLFWwindow *window, i32 focused);
void windowIconifyCallbackWrapper(GLFWwindow *window, i32 iconified);
void windowMaximizeCallbackWrapper(GLFWwindow *window, i32 maximized);
void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, i32 width,
                                          i32 height);
void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale);
void keyCallbackWrapper(GLFWwindow *window, i32 key, i32 scancode, i32 action,
                        i32 mods);
void charCallbackWrapper(GLFWwindow *window, u32 codepoint);
void dropCallbackWrapper(GLFWwindow *window, i32 count, const char **paths);
} // namespace Callbacks

class Window;

typedef Event<Window *, i32 const &, i32 const &> WindowPositionEvent;
typedef Event<Window *, i32 const &, i32 const &> WindowSizeEvent;
typedef Event<Window *> WindowCloseEvent;
typedef Event<Window *> WindowRefreshEvent;
typedef Event<Window *, bool const &> WindowFocusEvent;
typedef Event<Window *, bool const &> WindowIconifyEvent;
typedef Event<Window *, bool const &> WindowMaximizeEvent;
typedef Event<Window *, u32 const &, u32 const &> WindowFramebufferSizeEvent;
typedef Event<Window *, float const &, float const &> WindowContentScaleEvent;
typedef Event<Window *, Key const &> KeyEvent;
typedef Event<Window *, u32 const &> CharEvent;
typedef Event<Window *, vec<str> const &> FileDropEvent;

class WindowProperty {
private:
  PROHIBIT_COPY_AND_ASSIGN(WindowProperty);

private:
  GLFWwindow *mWindow = nullptr;

  vec<u32> mCodePoints;
  vec<Key> mKeys;
  vec<str> mDroppedFiles;

private:
  friend void Callbacks::charCallbackWrapper(GLFWwindow *, u32);
  friend void Callbacks::keyCallbackWrapper(GLFWwindow *, i32, i32, i32, i32);
  friend void Callbacks::dropCallbackWrapper(GLFWwindow *, i32, char const **);
  friend class Window;

  void setWindow(GLFWwindow *window) { mWindow = window; }
  void addCodePoint(u32 const &codePoint) { mCodePoints.push_back(codePoint); }
  void addKey(Key const &key) { mKeys.push_back(key); }
  void addDroppedFiles(vec<str> const &files) {
    mDroppedFiles.insert(mDroppedFiles.end(), files.begin(), files.end());
  }

public:
  WindowProperty() = default;
  ~WindowProperty() = default;

  pair<i32> getPosition() const;
  pair<u32> getSize() const;
  str getClipboard() const { return glfwGetClipboardString(mWindow); }
  str getTitle() const { return glfwGetWindowTitle(mWindow); }
  float getOpacity() const { return (float)glfwGetWindowOpacity(mWindow); }
  pair<u32> getFramebufferSize() const;
  pair<float> getContentScale() const;
  pair<i32> getMonitorPosition() const;
  pair<u32> getMonitorSize() const;
  pair<u32> getMonitorPhysicalSize() const;
  u32 getMonitorRefreshRate() const;
  str getMonitorName() const;
  vec<u32> const &getCodePoints() const { return mCodePoints; }
  vec<Key> const &getKeys() const { return mKeys; }
  vec<str> const &getDroppedFiles() const { return mDroppedFiles; }

  void setPosition(i32 const &xpos, i32 const &ypos) {
    glfwSetWindowPos(mWindow, xpos, ypos);
  }
  void setSize(u32 const &width, u32 const &height) {
    glfwSetWindowSize(mWindow, width, height);
  }
  void setClipboard(str const &text) {
    glfwSetClipboardString(mWindow, text.c_str());
  }
  void setTitle(str const &title) {
    glfwSetWindowTitle(mWindow, title.c_str());
  }
  void setOpacity(float const &opacity) {
    glfwSetWindowOpacity(mWindow, opacity);
  }

  void clearCodePoints() { mCodePoints.clear(); }
  void clearKeys() { mKeys.clear(); }
  void clearDroppedFiles() { mDroppedFiles.clear(); }
  void clearAllInputs();
};

class WindowEvent {
private:
  PROHIBIT_COPY_AND_ASSIGN(WindowEvent);

private:
  GLFWwindow *mWindow = nullptr;

private:
  friend class Window;

  void setWindow(GLFWwindow *window);

public:
  WindowPositionEvent onPositionChange;
  WindowSizeEvent onSizeChange;
  WindowCloseEvent onClose;
  WindowRefreshEvent onRefresh;
  WindowFocusEvent onFocus;
  WindowIconifyEvent onIconify;
  WindowMaximizeEvent onMaximize;
  WindowFramebufferSizeEvent onFramebufferSizeChange;
  WindowContentScaleEvent onContentScaleChange;
  KeyEvent onKeyInput;
  CharEvent onCharInput;
  FileDropEvent onFileDrop;

public:
  WindowEvent() = default;
  ~WindowEvent() = default;
};
} // namespace Terreate::OpenGL
