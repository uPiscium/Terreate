#ifndef __TC_WINDOW_HPP__
#define __TC_WINDOW_HPP__

#include "defines.hpp"
#include "logger.hpp"
#include "object.hpp"

namespace TerreateCore::Core {
using namespace TerreateCore::Defines;

class Icon final : public Object {
private:
  Vec<GLFWimage> mImages;
  Vec<Ubyte *> mPointers;

private:
  TC_DISABLE_COPY_AND_ASSIGN(Icon);

public:
  /*
   * @brief: This function creates a glfw icon.
   */
  Icon();
  ~Icon() override;

  Uint GetSize() const;

  /*
   * @brief: This function adds an image to the icon.
   * @param: width: Image width.
   * @param: height: Image height.
   * @param: pixels: Image pixels.
   * @detail: Pixels are copied to new allocated array and set to "glfw image"
   * instance. "glfw image" is used to render icon. Icon can have multiple of
   * images.
   */
  void AddImage(Uint const &width, Uint const &height, Ubyte const *pixels);

  operator GLFWimage const *() const { return mImages.data(); }
  operator Bool() const override { return mImages.size() > 0; }
};

class Cursor final : public Object {
private:
  GLFWcursor *mCursor = nullptr;
  GLFWimage mImage = GLFWimage();
  Ubyte *mPixels = nullptr;
  int mXHot = 0;
  int mYHot = 0;

private:
  TC_DISABLE_COPY_AND_ASSIGN(Cursor);

public:
  /*
   * @brief: This function creates a glfw cursor.
   * @param: xHot: The x-coordinate of the cursor's hot spot.
   * @param: yHot: The y-coordinate of the cursor's hot spot.
   */
  Cursor(Int const &xHot = 0, Int const &yHot = 0);
  ~Cursor() override;

  /*
   * @brief: This function sets the image of the cursor.
   * @param: width: Image width.
   * @param: height: Image height.
   * @param: pixels: Image pixels.
   * @detail: The image is copied to new allocated array and set to "glfw
   * image" instance. "glfw image" is used to render cursor.
   */
  void SetImage(Uint const &width, Uint const &height, Ubyte const *pixels);

  operator GLFWcursor *() const { return mCursor; }
  operator Bool() const override { return mCursor != nullptr; }
};

class StandardCursor : public Object {
private:
  GLFWcursor *mCursor = nullptr;

public:
  /*
   * @brief: This function creates a glfw standard cursor.
   * @param: shape: Cursor shape.
   */
  StandardCursor(CursorShape const &shape);
  ~StandardCursor() override;

  operator GLFWcursor *() const { return mCursor; }
  operator Bool() const override { return mCursor != nullptr; }
};

namespace Callbacks {
void WindowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos);
void WindowSizeCallbackWrapper(GLFWwindow *window, int width, int height);
void WindowCloseCallbackWrapper(GLFWwindow *window);
void WindowRefreshCallbackWrapper(GLFWwindow *window);
void WindowFocusCallbackWrapper(GLFWwindow *window, int focused);
void WindowIconifyCallbackWrapper(GLFWwindow *window, int iconified);
void WindowMaximizeCallbackWrapper(GLFWwindow *window, int maximized);
void WindowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                          int height);
void WindowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale);
void MousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
                                int mods);
void CursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos);
void CursorEnterCallbackWrapper(GLFWwindow *window, int entered);
void ScrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset);
void KeyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods);
void CharCallbackWrapper(GLFWwindow *window, Uint codepoint);
void DropCallbackWrapper(GLFWwindow *window, int count, const char **paths);
} // namespace Callbacks

struct WindowSettings {
public:
  Uint resizable = GLFW_TRUE;
  Uint visible = GLFW_TRUE;
  Uint decorated = GLFW_TRUE;
  Uint focused = GLFW_TRUE;
  Uint autoIconify = GLFW_TRUE;
  Uint floating = GLFW_FALSE;
  Uint maximized = GLFW_FALSE;
  Uint centerCursor = GLFW_TRUE;
  Uint transparentFramebuffer = GLFW_FALSE;
  Uint focusOnShow = GLFW_TRUE;
  Uint scaleToMonitor = GLFW_FALSE;
};

class Window;

class WindowProperty {
private:
  Str mTitle = "TerreateCore Window";
  Pair<Double> mScrollOffset;
  Vec<Uint> mCodePoints;
  Vec<Key> mKeys;
  Vec<Str> mDroppedFiles;
  Pair<Uint> mSize;
  Pair<Int> mPosition;
  Pair<Double> mCursorPosition;

  friend void Callbacks::ScrollCallbackWrapper(GLFWwindow *window,
                                               double xoffset, double yoffset);
  friend void Callbacks::CharCallbackWrapper(GLFWwindow *window,
                                             Uint codepoint);
  friend void Callbacks::KeyCallbackWrapper(GLFWwindow *window, int key,
                                            int scancode, int action, int mods);
  friend void Callbacks::DropCallbackWrapper(GLFWwindow *window, int count,
                                             const char **paths);
  friend void Callbacks::WindowSizeCallbackWrapper(GLFWwindow *window,
                                                   int width, int height);
  friend void Callbacks::WindowPositionCallbackWrapper(GLFWwindow *window,
                                                       int xpos, int ypos);
  friend void Callbacks::CursorPositionCallbackWrapper(GLFWwindow *window,
                                                       double xpos,
                                                       double ypos);
  friend Window;

public:
  WindowProperty() = default;
  ~WindowProperty() = default;

  /*
   * @brief: This function returns window title.
   * @return: Window title.
   */
  Str const &GetTitle() const;
  /*
   * @brief: This function returns scroll offset.
   * @return: Scroll offset.
   * @detail: Return format is (x, y).
   */
  Pair<Double> const &GetScrollOffset() const;
  /*
   * @brief: This function returns inputted code points.
   * @return: Inputted code points.
   * @detail: Code points are Unicode code points.
   */
  Vec<Uint> const &GetCodePoints() const;
  /*
   * @brief: This function returns inputted keys.
   * @return: Inputted keys.
   */
  Vec<Key> const &GetKeys() const;
  /*
   * @brief: This function returns dropped files.
   * @return: Dropped files.
   */
  Vec<Str> const &GetDroppedFiles() const;
  /*
   * @brief: This function returns window size.
   * @return: Window size.
   * @detail: Return format is (width, height).
   */
  Pair<Uint> const &GetSize() const;
  /*
   * @brief: This function returns window position.
   * @return: Window position.
   * @detail: Return format is (x, y).
   */
  Pair<Int> const &GetPosition() const;
  /*
   * @brief: This function returns cursor position.
   * @return: Cursor position.
   * @detail: Return format is (x, y).
   */
  Pair<Double> const &GetCursorPosition() const;

  /*
   * @brief: This function clears inputted chars, inputted codepoints, and
   * inputted keys.
   */
  void ClearInputs();
};

class WindowController {
public:
  virtual ~WindowController() = default;

  virtual void PositionCallback(int const &xpos, int const &ypos) {}
  virtual void SizeCallback(int const &width, int const &height) {}
  virtual void CloseCallback() {}
  virtual void RefreshCallback() {}
  virtual void FocusCallback(Bool const &focused) {}
  virtual void IconifyCallback(Bool const &iconified) {}
  virtual void MaximizeCallback(Bool const &maximized) {}
  virtual void FramebufferSizeCallback(int const &width, int const &height) {}
  virtual void ContentScaleCallback(float const &xscale, float const &yscale) {}
  virtual void MousebuttonCallback(Uint const &button, Uint const &action,
                                   Modifier const &mods) {}
  virtual void CursorPositionCallback(double const &xpos, double const &ypos) {}
  virtual void CursorEnterCallback(Bool const &entered) {}
  virtual void ScrollCallback(double const &xoffset, double const &yoffset) {}
  virtual void KeyCallback(Key const &key) {}
  virtual void CharCallback(Uint const &codepoint) {}
  virtual void DropCallback(Vec<Str> const &paths) {}

  virtual void OnFrame(Window *window) = 0;
};

class Window final : public Object {
private:
  GLFWwindow *mWindow = nullptr;
  void *mUserPointer = nullptr;
  WindowProperty mProperty;
  WindowController *mController = nullptr;

  friend void Callbacks::WindowPositionCallbackWrapper(GLFWwindow *window,
                                                       int xpos, int ypos);
  friend void Callbacks::WindowSizeCallbackWrapper(GLFWwindow *window,
                                                   int width, int height);
  friend void Callbacks::WindowCloseCallbackWrapper(GLFWwindow *window);
  friend void Callbacks::WindowRefreshCallbackWrapper(GLFWwindow *window);
  friend void Callbacks::WindowFocusCallbackWrapper(GLFWwindow *window,
                                                    int focused);
  friend void Callbacks::WindowIconifyCallbackWrapper(GLFWwindow *window,
                                                      int iconified);
  friend void Callbacks::WindowMaximizeCallbackWrapper(GLFWwindow *window,
                                                       int maximized);
  friend void
  Callbacks::WindowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                                  int height);
  friend void Callbacks::WindowContentScaleCallbackWrapper(GLFWwindow *window,
                                                           float xscale,
                                                           float yscale);
  friend void Callbacks::MousebuttonCallbackWrapper(GLFWwindow *window,
                                                    int button, int action,
                                                    int mods);
  friend void Callbacks::CursorPositionCallbackWrapper(GLFWwindow *window,
                                                       double xpos,
                                                       double ypos);
  friend void Callbacks::CursorEnterCallbackWrapper(GLFWwindow *window,
                                                    int entered);
  friend void Callbacks::ScrollCallbackWrapper(GLFWwindow *window,
                                               double xoffset, double yoffset);
  friend void Callbacks::KeyCallbackWrapper(GLFWwindow *window, int key,
                                            int scancode, int action, int mods);
  friend void Callbacks::CharCallbackWrapper(GLFWwindow *window,
                                             Uint codepoint);
  friend void Callbacks::DropCallbackWrapper(GLFWwindow *window, int count,
                                             const char **paths);

private:
  TC_DISABLE_COPY_AND_ASSIGN(Window);

public:
  /*
   * @brief: This function creates a glfw window and sets the callbacks.
   * @param: width: Window width.
   * @param: height: Window height.
   * @param: title: Window title.
   * @param: settings: Window settings.
   */
  Window(Uint const &width, Uint const &height, Str const &title,
         WindowSettings const &settings);
  ~Window() override;

  /*
   * @brief: This function returns window size.
   * @return: Window size.
   * @detail: Return format is (width, height).
   */
  Pair<Uint> const &GetSize() const;
  /*
   * @brief: This function returns window position.
   * @return: Window position.
   * @detail: Return format is (x, y).
   */
  Pair<Int> const &GetPosition() const;
  /*
   * @brief: This function returns cursor position.
   * @return: Cursor position.
   * @detail: Return format is (x, y)
   */
  Pair<Double> const &GetCursorPosition() const;
  /*
   * @brief: This function returns scroll offset.
   * @return: Scroll offset.
   * @detail: Return format is (x, y).
   * @note: Scroll offset cannot be set.
   */
  Pair<Double> const &GetScrollOffset() const;
  /*
   * @brief: This function returns clipboard string.
   * @return: Clipboard string.
   */
  Str GetClipboardString() const;
  /*
   * @brief: This function returns window title.
   * @return: Window title.
   */
  Str const &GetTitle() const;
  /*
   * @brief: This function returns inputted code points.
   * @return: Inputted code points.
   * @sa: ClearInputs()
   * @detail: Code points are Unicode code points. This function doesn't clear
   * inputted code points. If you want to clear inputted code points, use
   * ClearInputs() function.
   */
  Vec<Uint> const &GetCodePoints() const;
  /*
   * @brief: This function returns inputted keys.
   * @return: Inputted keys.
   * @sa: ClearInputs()
   * @detail: This function doesn't clear inputted keys. If you want to clear
   * inputted keys, use ClearInputs() function.
   */
  Vec<Key> const &GetKeys() const;
  /*
   * @brief: This function returns dropped files.
   * @return: Dropped files.
   * @sa: ClearInputs()
   * @detail: This function doesn't clear dropped files. If you want to clear
   * dropped files, use ClearInputs() function.
   */
  Vec<Str> const &GetDroppedFiles() const;
  /*
   * @brief: This function returns window opacity.
   * @return: Window opacity.
   */
  Float GetOpacity() const;
  /*
   * @brief: This function returns specified mouse button state.
   * @return: Mousebutton state [true(Pressed)/false(Released)].
   */
  Bool GetMousebutton(MousebuttonInput const &button) const;
  /*
   * @brief: This function returns specified input type state.
   * @return: Input type state. [true(enabled)/false(disabled)]
   * @note: If you want to get cursor mode, use GetCursorMode() function.
   */
  Bool GetInputTypeState(InputType const &type) const;
  /*
   * @brief: This function returns specified input type state.
   * @return: Current cursor state.
   */
  CursorMode GetCursorMode() const;
  /*
   * @brief: This function returns user pointer binded to window.
   * @return: User pointer.
   */
  template <typename T> T *GetUserPointer() const {
    TC_TRACE_CALL(LOCATION(Window));
    return static_cast<T *>(mUserPointer);
  }

  /*
   * @brief: This function sets window as current context.
   */
  void SetCurrentContext() const;
  /*
   * @brief: This function sets window icon.
   * @param: icon: Window icon.
   */
  void SetIcon(Icon const &icon);
  /*
   * @brief: This function sets cursor appearance.
   * @param: cursor: Cursor data.
   */
  void SetCursor(Cursor const &cursor);
  /*
   * @brief: This function sets standard cursor appearance.
   * @param: cursor: Standard cursor data.
   */
  void SetCursor(StandardCursor const &cursor);
  /*
   * @brief: This function sets window cursor to default.
   */
  void SetDefaultCursor();
  /*
   * @brief: This function sets window size.
   * @param: size: Window size.
   * @detail: Size format is (width, height).
   */
  void SetSize(Pair<Uint> const &size);
  /*
   * @brief: This function sets window position.
   * @param: position: Window position.
   * @detail: Position format is (x, y).
   */
  void SetPosition(Pair<int> const &position);
  /*
   * @brief: This function sets cursor position.
   * @param: position: Cursor position.
   * @detail: Position format is (x, y).
   */
  void SetCursorPosition(Pair<Double> const &position);
  /*
   * @brief: This function sets clipboard string.
   * @param: string: Clipboard string.
   */
  void SetClipboardString(Str const &string);
  /*
   * @brief: This function sets window title.
   * @param: title: Window title.
   */
  void SetTitle(Str const &title);
  /*
   * @brief: This function sets window opacity.
   * @param: opacity: Window opacity.
   */
  void SetOpacity(float const &opacity);
  /*
   * @brief: This function sets specified input type state.
   * @param: type: Input type.
   * @param: state: Input type state.
   */
  void SetInputTypeState(InputType const &type, Bool const &state);
  /*
   * @brief: This function sets cursor mode.
   * @param: mode: Cursor mode.
   */
  void SetCursorMode(CursorMode const &mode);
  /*
   * This function sets user pointer binded to window.
   * @param: pointer: User pointer.
   */
  template <typename T> void SetUserPointer(T *pointer) {
    TC_TRACE_CALL(LOCATION(Window));
    mUserPointer = static_cast<void *>(pointer);
  }
  /*
   * @brief: This function sets window callbacks.
   * @param: callbacks: Window callbacks.
   */
  void SetWindowController(WindowController *callbacks);

  /*
   * @brief: This function returns whether window is closed or not.
   * @return: Whether window is closed or not.
   */
  Bool IsClosed() const {
    return mWindow == nullptr || glfwWindowShouldClose(mWindow);
  }
  /*
   * @brief: This function returns whether window is fullscreen or not.
   * @return: Whether window is fullscreen or not.
   */
  Bool IsFullScreen() const {
    return Bool(glfwGetWindowMonitor(mWindow) != NULL);
  }
  /*
   * @brief: This function returns whether window is windowed or not.
   * @return: Whether window is windowed or not.
   */
  Bool IsWindowed() const {
    return Bool(glfwGetWindowMonitor(mWindow) == NULL);
  }
  /*
   * @brief: This function returns whether window is iconified or not.
   * @return: Whether window is iconified or not.
   */
  Bool IsIconified() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_ICONIFIED));
  }
  /*
   * @brief: This function returns whether window is maximized or not.
   * @return: Whether window is maximized or not.
   */
  Bool IsMaximized() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_MAXIMIZED));
  }
  /*
   * @brief: This function returns whether window is visible or not.
   * @return: Whether window is visible or not.
   */
  Bool IsVisible() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_VISIBLE));
  }
  /*
   * @brief: This function returns whether window is focused or not.
   * @return: Whether window is focused or not.
   */
  Bool IsFocused() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUSED));
  }
  /*
   * @brief: This function returns whether window is transparent or not.
   * @return: Whether window is transparent or not.
   */
  Bool IsTransparent() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_TRANSPARENT_FRAMEBUFFER));
  }
  /*
   * @brief: This function returns whether window is decorated or not.
   * @return: Whether window is decorated or not.
   */
  Bool IsDecorated() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_DECORATED));
  }
  /*
   * @brief: This function returns whether window is resizable or not.
   * @return: Whether window is resizable or not.
   */
  Bool IsResizable() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_RESIZABLE));
  }
  /*
   * @brief: This function returns whether window is floating or not.
   * @return: Whether window is floating or not.
   */
  Bool IsFloating() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_FLOATING));
  }
  /*
   * @brief: This function returns whether window is auto iconified or not.
   * @return: Whether window is auto iconified or not.
   * @detail: "Auto iconify" specifies whether the full screen window will
   * automatically iconify and restore the previous video mode on input focus
   * loss.
   */
  Bool IsAutoIconified() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_AUTO_ICONIFY));
  }
  /*
   * @brief: This function returns whether window is focus on showed or not.
   * @return: Whether window is maximized or not.
   * @sa: Show()
   * @detail: "Focus on show" specifies whether the window will be given input
   * focus when Show() function is called.
   */
  Bool IsFocusOnShowed() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUS_ON_SHOW));
  }
  /*
   * @brief: This function returns whether key is pressing or not.
   * @return: Whether key is pressing or not.
   */
  Bool IsPressing(Keyboard const &key) const {
    return Bool(glfwGetKey(mWindow, (Uint)key) == GLFW_PRESS);
  }
  /*
   * @brief: This function returns whether cursor is entering or not.
   * @return: Whether cursor is entering or not.
   */
  Bool IsEntering() const {
    return Bool(glfwGetWindowAttrib(mWindow, GLFW_HOVERED));
  }

  /*
   * @brief: This function closes window.
   */
  void Close();
  /*
   * @brief: This function iconifies window.
   */
  void Iconify() const;
  /*
   * @brief: This function maximizes window.
   */
  void Maximize() const;
  /*
   * @brief: This function shows window.
   */
  void Show() const;
  /*
   * @brief: This function hides window.
   */
  void Hide() const;
  /*
   * @brief: This function focuses window.
   */
  void Focus() const;
  /*
   * @brief: This function unmaximize or uniconify window.
   */
  void Restore() const;
  /*
   * @brief: This function requests attention to window.
   */
  void RequestAttention() const;
  /*
   * @brief: This function fills window with specified color.
   * @param: color: Fill color.
   * @detail: Color format is (red, green, blue). Each color is float (0
   * ~ 1.0).
   */
  void Fill(Vec<float> const &color) const;
  /*
   * @brief: This function clears color, depth, and stencil buffers.
   */
  void Clear() const;
  /*
   * @brief: This function clears color buffer.
   */
  void ClearColor() const;
  /*
   * @brief: This function clears depth buffer.
   */
  void ClearDepth() const;
  /*
   * @brief: This function clears stencil buffer.
   */
  void ClearStencil() const;
  /*
   * @brief: This function swaps front and back buffers.
   */
  void Swap() const;
  /*
   * @brief: This function poll events.
   */
  void PollEvents() const;
  /*
   * @brief: This function binds window to current context.
   */
  void Bind() const;
  /*
   * @brief: This function executes callbacks->Run().
   */
  void Frame();

  operator Bool() const override { return !this->IsClosed(); }
};
} // namespace TerreateCore::Core

#endif // __TC_WINDOW_HPP__
