#ifndef __TERREATE_GRAPHICS_WINDOW_HPP__
#define __TERREATE_GRAPHICS_WINDOW_HPP__

#include "defines.hpp"

namespace TerreateGraphics::Core {
using namespace TerreateGraphics::Defines;

class Window;

typedef Event<Window *, Uint const &, Uint const &> WindowPositionEvent;
typedef Event<Window *, Uint const &, Uint const &> WindowSizeEvent;
typedef Event<Window *> WindowCloseEvent;
typedef Event<Window *> WindowRefreshEvent;
typedef Event<Window *, Bool const &> WindowFocusEvent;
typedef Event<Window *, Bool const &> WindowIconifyEvent;
typedef Event<Window *, Bool const &> WindowMaximizeEvent;
typedef Event<Window *, Uint const &, Uint const &> WindowFramebufferSizeEvent;
typedef Event<Window *, Float const &, Float const &> WindowContentScaleEvent;
typedef Event<Window *, Uint const &, Uint const &, Modifier const &>
    MousebuttonEvent;
typedef Event<Window *, Double const &, Double const &> CursorPositionEvent;
typedef Event<Window *, Bool const &> CursorEnterEvent;
typedef Event<Window *, Double const &, Double const &> ScrollEvent;
typedef Event<Window *, Key const &> KeyEvent;
typedef Event<Window *, Uint const &> CharEvent;
typedef Event<Window *, Vec<Str> const &> FileDropEvent;

class Icon final : public TerreateObjectBase {
private:
  Vec<GLFWimage> mImages;
  Vec<Ubyte *> mPointers;

private:
  Icon(Icon const &) = delete;
  Icon &operator=(Icon const &) = delete;

public:
  /*
   * @brief: This function creates a glfw icon.
   */
  Icon();
  ~Icon() override;

  Uint GetSize() const { return mImages.size(); }

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

class Cursor final : public TerreateObjectBase {
private:
  GLFWcursor *mCursor = nullptr;
  GLFWimage mImage = GLFWimage();
  Ubyte *mPixels = nullptr;
  int mXHot = 0;
  int mYHot = 0;

private:
  Cursor(Cursor const &) = delete;
  Cursor &operator=(Cursor const &) = delete;

public:
  /*
   * @brief: This function creates a glfw cursor.
   * @param: xHot: The x-coordinate of the cursor's hot spot.
   * @param: yHot: The y-coordinate of the cursor's hot spot.
   */
  Cursor(Int const &xHot = 0, Int const &yHot = 0) : mXHot(xHot), mYHot(yHot) {}
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

class StandardCursor : public TerreateObjectBase {
private:
  GLFWcursor *mCursor = nullptr;

private:
  StandardCursor(StandardCursor const &) = delete;
  StandardCursor &operator=(StandardCursor const &) = delete;

public:
  /*
   * @brief: This function creates a glfw standard cursor.
   * @param: shape: Cursor shape.
   */
  StandardCursor(CursorShape const &shape) {
    mCursor = glfwCreateStandardCursor((Uint)shape);
  }
  ~StandardCursor() override { glfwDestroyCursor(mCursor); }

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

class WindowProperty {
private:
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
   * @brief: This function returns scroll offset.
   * @return: Scroll offset.
   * @detail: Return format is (x, y).
   */
  Pair<Double> const &GetScrollOffset() const { return mScrollOffset; }
  /*
   * @brief: This function returns inputted code points.
   * @return: Inputted code points.
   * @detail: Code points are Unicode code points.
   */
  Vec<Uint> const &GetCodePoints() const { return mCodePoints; }
  /*
   * @brief: This function returns inputted keys.
   * @return: Inputted keys.
   */
  Vec<Key> const &GetKeys() const { return mKeys; }
  /*
   * @brief: This function returns dropped files.
   * @return: Dropped files.
   */
  Vec<Str> const &GetDroppedFiles() const { return mDroppedFiles; }
  /*
   * @brief: This function returns window size.
   * @return: Window size.
   * @detail: Return format is (width, height).
   */
  Pair<Uint> const &GetSize() const { return mSize; }
  /*
   * @brief: This function returns window position.
   * @return: Window position.
   * @detail: Return format is (x, y).
   */
  Pair<Int> const &GetPosition() const { return mPosition; }
  /*
   * @brief: This function returns cursor position.
   * @return: Cursor position.
   * @detail: Return format is (x, y).
   */
  Pair<Double> const &GetCursorPosition() const { return mCursorPosition; }

  /*
   * @brief: This function clears inputted chars, inputted codepoints, and
   * inputted keys.
   */
  void ClearInputs();
};

class Window final : public TerreateObjectBase {
private:
  GLFWwindow *mWindow = nullptr;
  WindowProperty mProperty;

  WindowPositionEvent mPositionEvent;
  WindowSizeEvent mSizeEvent;
  WindowCloseEvent mCloseEvent;
  WindowRefreshEvent mRefreshEvent;
  WindowFocusEvent mFocusEvent;
  WindowIconifyEvent mIconifyEvent;
  WindowMaximizeEvent mMaximizeEvent;
  WindowFramebufferSizeEvent mFramebufferSizeEvent;
  WindowContentScaleEvent mContentScaleEvent;
  MousebuttonEvent mMousebuttonEvent;
  CursorPositionEvent mCursorPositionEvent;
  CursorEnterEvent mCursorEnterEvent;
  ScrollEvent mScrollEvent;
  KeyEvent mKeyEvent;
  CharEvent mCharEvent;
  FileDropEvent mDropEvent;

private:
  Window(Window const &) = delete;
  Window &operator=(Window const &) = delete;

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
  ~Window() override { this->Destroy(); }

  /*
   * @brief: This function returns window property class.
   * @return: Window property class.
   */
  WindowProperty &GetProperty() { return mProperty; }
  /*
   * @brief: This function returns window property class.
   * @return: Window property class.
   */
  WindowProperty const &GetProperty() const { return mProperty; }
  /*
   * @brief: This function returns window position event.
   * @return: Window position event handler.
   */
  WindowPositionEvent &GetPositionEventHandler() { return mPositionEvent; }
  /*
   * @brief: This function returns window size event.
   * @return: Window size event handler.
   */
  WindowSizeEvent &GetSizeEventHandler() { return mSizeEvent; }
  /*
   * @brief: This function returns window close event.
   * @return: Window close event handler.
   */
  WindowCloseEvent &GetCloseEventHandler() { return mCloseEvent; }
  /*
   * @brief: This function returns window refresh event.
   * @return: Window refresh event handler.
   */
  WindowRefreshEvent &GetRefreshEventHandler() { return mRefreshEvent; }
  /*
   * @brief: This function returns window focus event.
   * @return: Window focus event handler.
   */
  WindowFocusEvent &GetFocusEventHandler() { return mFocusEvent; }
  /*
   * @brief: This function returns window iconify event.
   * @return: Window iconify event handler.
   */
  WindowIconifyEvent &GetIconifyEventHandler() { return mIconifyEvent; }
  /*
   * @brief: This function returns window maximize event.
   * @return: Window maximize event handler.
   */
  WindowMaximizeEvent &GetMaximizeEventHandler() { return mMaximizeEvent; }
  /*
   * @brief: This function returns window framebuffer size event.
   * @return: Window framebuffer size event handler.
   */
  WindowFramebufferSizeEvent &GetFramebufferSizeEventHandler() {
    return mFramebufferSizeEvent;
  }
  /*
   * @brief: This function returns window content scale event.
   * @return: Window content scale event handler.
   */
  WindowContentScaleEvent &GetContentScaleEventHandler() {
    return mContentScaleEvent;
  }
  /*
   * @brief: This function returns mousebutton event.
   * @return: Mousebutton event handler.
   */
  MousebuttonEvent &GetMousebuttonEventHandler() { return mMousebuttonEvent; }
  /*
   * @brief: This function returns cursor position event.
   * @return: Cursor position event handler.
   */
  CursorPositionEvent &GetCursorPositionEventHandler() {
    return mCursorPositionEvent;
  }
  /*
   * @brief: This function returns cursor enter event.
   * @return: Cursor enter event handler.
   */
  CursorEnterEvent &GetCursorEnterEventHandler() { return mCursorEnterEvent; }
  /*
   * @brief: This function returns scroll event.
   * @return: Scroll event handler.
   */
  ScrollEvent &GetScrollEventHandler() { return mScrollEvent; }
  /*
   * @brief: This function returns key event.
   * @return: Key event handler.
   */
  KeyEvent &GetKeyEventHandler() { return mKeyEvent; }
  /*
   * @brief: This function returns char event.
   * @return: Char event handler.
   */
  CharEvent &GetCharEventHandler() { return mCharEvent; }
  /*
   * @brief: This function returns drop event.
   * @return: Drop event handler.
   */
  FileDropEvent &GetFileDropEventHandler() { return mDropEvent; }

  /*
   * @brief: This function returns window size.
   * @return: Window size.
   * @detail: Return format is (width, height).
   */
  Pair<Uint> const &GetSize() const { return mProperty.GetSize(); }
  /*
   * @brief: This function returns window position.
   * @return: Window position.
   * @detail: Return format is (x, y).
   */
  Pair<Int> const &GetPosition() const { return mProperty.GetPosition(); }
  /*
   * @brief: This function returns cursor position.
   * @return: Cursor position.
   * @detail: Return format is (x, y)
   */
  Pair<Double> const &GetCursorPosition() const {
    return mProperty.GetCursorPosition();
  }
  /*
   * @brief: This function returns scroll offset.
   * @return: Scroll offset.
   * @detail: Return format is (x, y).
   * @note: Scroll offset cannot be set.
   */
  Pair<Double> const &GetScrollOffset() const {
    return mProperty.GetScrollOffset();
  }
  /*
   * @brief: This function returns clipboard string.
   * @return: Clipboard string.
   */
  Str GetClipboardString() const { return glfwGetClipboardString(mWindow); }
  /*
   * @brief: This function returns window title.
   * @return: Window title.
   */
  Str GetTitle() const { return Str(glfwGetWindowTitle(mWindow)); }
  /*
   * @brief: This function returns inputted code points.
   * @return: Inputted code points.
   * @sa: ClearInputs()
   * @detail: Code points are Unicode code points. This function doesn't clear
   * inputted code points. If you want to clear inputted code points, use
   * ClearInputs() function.
   */
  Vec<Uint> const &GetCodePoints() const { return mProperty.GetCodePoints(); }
  /*
   * @brief: This function returns inputted keys.
   * @return: Inputted keys.
   * @sa: ClearInputs()
   * @detail: This function doesn't clear inputted keys. If you want to clear
   * inputted keys, use ClearInputs() function.
   */
  Vec<Key> const &GetKeys() const { return mProperty.GetKeys(); }
  /*
   * @brief: This function returns dropped files.
   * @return: Dropped files.
   * @sa: ClearInputs()
   * @detail: This function doesn't clear dropped files. If you want to clear
   * dropped files, use ClearInputs() function.
   */
  Vec<Str> const &GetDroppedFiles() const {
    return mProperty.GetDroppedFiles();
  }
  /*
   * @brief: This function returns window opacity.
   * @return: Window opacity.
   */
  Float GetOpacity() const { return glfwGetWindowOpacity(mWindow); }
  /*
   * @brief: This function returns specified mouse button state.
   * @return: Mousebutton state [true(Pressed)/false(Released)].
   */
  Bool GetMousebutton(MousebuttonInput const &button) const {
    return Bool(glfwGetMouseButton(mWindow, (Uint)button) == GLFW_PRESS);
  }
  /*
   * @brief: This function returns specified input type state.
   * @return: Input type state. [true(enabled)/false(disabled)]
   * @note: If you want to get cursor mode, use GetCursorMode() function.
   */
  Bool GetInputTypeState(InputType const &type) const {
    return Bool(glfwGetInputMode(mWindow, (Uint)type) == GLFW_TRUE);
  }
  /*
   * @brief: This function returns specified input type state.
   * @return: Current cursor state.
   */
  CursorMode GetCursorMode() const {
    return static_cast<CursorMode>(glfwGetInputMode(mWindow, GLFW_CURSOR));
  }

  /*
   * @brief: This function sets window as current context.
   */
  void SetCurrentContext() const { glfwMakeContextCurrent(mWindow); }
  /*
   * @brief: This function sets window icon.
   * @param: icon: Window icon.
   */
  void SetIcon(Icon const &icon) {
    glfwSetWindowIcon(mWindow, icon.GetSize(), (GLFWimage const *)icon);
  }
  /*
   * @brief: This function sets cursor appearance.
   * @param: cursor: Cursor data.
   */
  void SetCursor(Cursor const &cursor) {
    glfwSetCursor(mWindow, (GLFWcursor *)cursor);
  }
  /*
   * @brief: This function sets standard cursor appearance.
   * @param: cursor: Standard cursor data.
   */
  void SetCursor(StandardCursor const &cursor) {
    glfwSetCursor(mWindow, (GLFWcursor *)cursor);
  }
  /*
   * @brief: This function sets window cursor to default.
   */
  void SetDefaultCursor() { glfwSetCursor(mWindow, nullptr); }
  /*
   * @brief: This function sets window size.
   * @param: size: Window size.
   * @detail: Size format is (width, height).
   */
  void SetSize(Pair<Uint> const &size) {
    glfwSetWindowSize(mWindow, size.first, size.second);
  }
  /*
   * @brief: This function sets window position.
   * @param: position: Window position.
   * @detail: Position format is (x, y).
   */
  void SetPosition(Pair<int> const &position) {
    glfwSetWindowPos(mWindow, position.first, position.second);
  }
  /*
   * @brief: This function sets cursor position.
   * @param: position: Cursor position.
   * @detail: Position format is (x, y).
   */
  void SetCursorPosition(Pair<Double> const &position) {
    glfwSetCursorPos(mWindow, position.first, position.second);
  }
  /*
   * @brief: This function sets clipboard string.
   * @param: string: Clipboard string.
   */
  void SetClipboardString(Str const &string) {
    glfwSetClipboardString(mWindow, string.c_str());
  }
  /*
   * @brief: This function sets window title.
   * @param: title: Window title.
   */
  void SetTitle(Str const &title) {
    glfwSetWindowTitle(mWindow, title.c_str());
  }
  /*
   * @brief: This function sets window opacity.
   * @param: opacity: Window opacity.
   */
  void SetOpacity(float const &opacity) {
    glfwSetWindowOpacity(mWindow, opacity);
  }
  /*
   * @brief: This function sets specified input type state.
   * @param: type: Input type.
   * @param: state: Input type state.
   */
  void SetInputTypeState(InputType const &type, Bool const &state) {
    glfwSetInputMode(mWindow, (int)type, state);
  }
  /*
   * @brief: This function sets cursor mode.
   * @param: mode: Cursor mode.
   */
  void SetCursorMode(CursorMode const &mode) {
    glfwSetInputMode(mWindow, GLFW_CURSOR, (int)mode);
  }
  /*
   * @brief: This function sets window view port.
   * @param: x0: View port x0.
   * @param: y0: View port y0.
   * @param: width: View port width.
   * @param: height: View port height.
   */
  void SetViewPort(Uint const &x0, Uint const &y0, Uint const &width,
                   Uint const &height) {
    glViewport(x0, y0, width, height);
  }
  /*
   * @brief: This function sets window view port.
   * @param: width: View port width.
   * @param: height: View port height.
   */
  void SetViewPort(Uint const &width, Uint const &height) {
    glViewport(0, 0, width, height);
  }

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
   * @brief: This function enables window vsync.
   */
  void EnableVsync() const { glfwSwapInterval(1); }
  /*
   * @brief: This function disables window vsync.
   */
  void DisableVsync() const { glfwSwapInterval(0); }
  /*
   * @brief: This function destroies window.
   */
  void Destroy();
  /*
   * @brief: This function closes window.
   */
  void Close() const { glfwSetWindowShouldClose(mWindow, GLFW_TRUE); }
  /*
   * @brief: This function iconifies window.
   */
  void Iconify() const { glfwIconifyWindow(mWindow); }
  /*
   * @brief: This function maximizes window.
   */
  void Maximize() const { glfwMaximizeWindow(mWindow); }
  /*
   * @brief: This function shows window.
   */
  void Show() const { glfwShowWindow(mWindow); }
  /*
   * @brief: This function hides window.
   */
  void Hide() const { glfwHideWindow(mWindow); }
  /*
   * @brief: This function focuses window.
   */
  void Focus() const { glfwFocusWindow(mWindow); }
  /*
   * @brief: This function unmaximize or uniconify window.
   */
  void Restore() const { glfwRestoreWindow(mWindow); }
  /*
   * @brief: This function requests attention to window.
   */
  void RequestAttention() const { glfwRequestWindowAttention(mWindow); }
  /*
   * @brief: This function fills window with specified color.
   * @param: color: Fill color.
   * @detail: Color format is (red, green, blue). Each color is float (0
   * ~ 1.0).
   */
  void Fill(Vec<float> const &color) const {
    glClearColor(color[0], color[1], color[2], 0.0f);
  }
  /*
   * @brief: This function clears color, depth, and stencil buffers.
   */
  void Clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }
  /*
   * @brief: This function clears color buffer.
   */
  void ClearColor() const { glClear(GL_COLOR_BUFFER_BIT); }
  /*
   * @brief: This function clears depth buffer.
   */
  void ClearDepth() const { glClear(GL_DEPTH_BUFFER_BIT); }
  /*
   * @brief: This function clears stencil buffer.
   */
  void ClearStencil() const { glClear(GL_STENCIL_BUFFER_BIT); }
  /*
   * @brief: This function swaps front and back buffers.
   */
  void Swap() const { glfwSwapBuffers(mWindow); }
  /*
   * @brief: This function poll events.
   */
  void PollEvents() const { glfwPollEvents(); }
  /*
   * @brief: This function binds window to current context.
   */
  void Bind() const { this->SetCurrentContext(); }
  /*
   * @brief: This function executes callbacks->Run().
   */
  void Frame(Function<void(Window *)> const &frameFunction);

  operator Bool() const override { return !this->IsClosed(); }
};
} // namespace TerreateGraphics::Core

#endif // __TERREATE_GRAPHICS_WINDOW_HPP__
