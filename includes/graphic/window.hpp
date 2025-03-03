#ifndef __TERREATE_GRAPHICS_WINDOW_HPP__
#define __TERREATE_GRAPHICS_WINDOW_HPP__

#include <core/property.hpp>
#include <graphic/GLdefs.hpp>
#include <io/image.hpp>
#include <types.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;
using namespace Terreate::Core;

struct Modifier {
public:
  TCbool shift = false;
  TCbool control = false;
  TCbool alt = false;
  TCbool super = false;
  TCbool capsLock = false;
  TCbool numLock = false;

public:
  Modifier(int mods_)
      : shift(mods_ & GLFW_MOD_SHIFT), control(mods_ & GLFW_MOD_CONTROL),
        alt(mods_ & GLFW_MOD_ALT), super(mods_ & GLFW_MOD_SUPER),
        capsLock(mods_ & GLFW_MOD_CAPS_LOCK),
        numLock(mods_ & GLFW_MOD_NUM_LOCK) {}
};

struct Key {
public:
  Keyboard key = Keyboard::K_LAST;
  TCi32 scancode = 0;
  Bool pressed = 0;
  Modifier mods = 0;

public:
  Key(int key_, int scancode_, int action_, int mods_)
      : key((Keyboard)key_), scancode(scancode_), pressed((Bool)action_),
        mods(mods_) {}
};

class Window;

typedef Event<Window *, Int const &, Int const &> WindowPositionEvent;
typedef Event<Window *, Int const &, Int const &> WindowSizeEvent;
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

namespace Subscribers {
typedef Subscriber<Window *, Int const &, Int const &> WindowPositionSubscriber;
typedef Subscriber<Window *, Int const &, Int const &> WindowSizeSubscriber;
typedef Subscriber<Window *> WindowCloseSubscriber;
typedef Subscriber<Window *> WindowRefreshSubscriber;
typedef Subscriber<Window *, Bool const &> WindowFocusSubscriber;
typedef Subscriber<Window *, Bool const &> WindowIconifySubscriber;
typedef Subscriber<Window *, Bool const &> WindowMaximizeSubscriber;
typedef Subscriber<Window *, Uint const &, Uint const &>
    WindowFramebufferSizeSubscriber;
typedef Subscriber<Window *, Float const &, Float const &>
    WindowContentScaleSubscriber;
typedef Subscriber<Window *, Uint const &, Uint const &, Modifier const &>
    MousebuttonSubscriber;
typedef Subscriber<Window *, Double const &, Double const &>
    CursorPositionSubscriber;
typedef Subscriber<Window *, Bool const &> CursorEnterSubscriber;
typedef Subscriber<Window *, Double const &, Double const &> ScrollSubscriber;
typedef Subscriber<Window *, Key const &> KeySubscriber;
typedef Subscriber<Window *, Uint const &> CharSubscriber;
typedef Subscriber<Window *, Vec<Str> const &> FileDropSubscriber;
} // namespace Subscribers

class Icon final {
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
  ~Icon();

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
  /*
   * @brief: This function adds an image to the icon.
   * @param: texture: Image texture.
   * @detail: Pixels are copied to new allocated array and set to "glfw image"
   * instance. "glfw image" is used to render icon. Icon can have multiple of
   * images.
   */
  void AddImage(IO::Texture const &texture) {
    this->AddImage(texture.width, texture.height, texture.data.data());
  }

  operator GLFWimage const *() const { return mImages.data(); }
  operator Bool() const { return mImages.size() > 0; }
};

class Cursor final {
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
  ~Cursor();

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
  operator Bool() const { return mCursor != nullptr; }
};

class StandardCursor {
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
  ~StandardCursor() { glfwDestroyCursor(mCursor); }

  operator GLFWcursor *() const { return mCursor; }
  operator Bool() const { return mCursor != nullptr; }
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
  Bool resizable = true;
  Bool visible = true;
  Bool decorated = true;
  Bool focused = true;
  Bool autoIconify = true;
  Bool floating = false;
  Bool maximized = false;
  Bool centerCursor = true;
  Bool transparentFramebuffer = false;
  Bool focusOnShow = true;
  Bool scaleToMonitor = false;
};

class Window {
private:
  GLFWwindow *mWindow = nullptr;

  // Read-only properties
  Pair<Double> mScrollOffset;
  Vec<Uint> mCodePoints;
  Vec<Key> mKeys;
  Vec<Str> mDroppedFiles;

  // Read-write properties
  Pair<Int> mSize;
  Pair<Int> mPosition;
  Pair<Double> mCursorPosition;
  Str mClipboard;
  Str mTitle;
  Float mOpacity;

public:
  // Read-only properties
  Property<Pair<Double>> scrollOffset;
  Property<Vec<Uint>> codePoints;
  Property<Vec<Key>> keys;
  Property<Vec<Str>> droppedFiles;
  Property<CursorMode> cursorMode;

  // Read-write properties
  Property<Pair<Int>> size;
  Property<Pair<Int>> position;
  Property<Pair<Double>> cursorPosition;
  Property<Str> clipboard;
  Property<Str> title;
  Property<Float> opacity;

  // Event handlers
  WindowPositionEvent onPositionChange;
  WindowSizeEvent onSizeChange;
  WindowCloseEvent onClose;
  WindowRefreshEvent onRefresh;
  WindowFocusEvent onFocus;
  WindowIconifyEvent onIconify;
  WindowMaximizeEvent onMaximize;
  WindowFramebufferSizeEvent onFramebufferSizeChange;
  WindowContentScaleEvent onContentScaleChange;
  MousebuttonEvent onMousebuttonInput;
  CursorPositionEvent onCursorPositionChange;
  CursorEnterEvent onCursorEnter;
  ScrollEvent onScroll;
  KeyEvent onKeyInput;
  CharEvent onCharInput;
  FileDropEvent onFileDrop;

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
   * @param: shared: GL context to share resources with.
   */
  Window(Uint const &width, Uint const &height, Str const &title,
         WindowSettings const &settings, Window *shared = nullptr);
  ~Window() { this->Destroy(); }

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
   * @brief: This function sets specified input type state.
   * @param: type: Input type.
   * @param: state: Input type state.
   */
  void SetInputTypeState(InputType const &type, Bool const &state) {
    glfwSetInputMode(mWindow, (int)type, state);
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
  Bool IsClosed() const;
  /*
   * @brief: This function returns whether window is fullscreen or not.
   * @return: Whether window is fullscreen or not.
   */
  Bool IsFullScreen() const;
  /*
   * @brief: This function returns whether window is windowed or not.
   * @return: Whether window is windowed or not.
   */
  Bool IsWindowed() const;
  /*
   * @brief: This function returns whether window is iconified or not.
   * @return: Whether window is iconified or not.
   */
  Bool IsIconified() const;
  /*
   * @brief: This function returns whether window is maximized or not.
   * @return: Whether window is maximized or not.
   */
  Bool IsMaximized() const;
  /*
   * @brief: This function returns whether window is visible or not.
   * @return: Whether window is visible or not.
   */
  Bool IsVisible() const;
  /*
   * @brief: This function returns whether window is focused or not.
   * @return: Whether window is focused or not.
   */
  Bool IsFocused() const;
  /*
   * @brief: This function returns whether window is transparent or not.
   * @return: Whether window is transparent or not.
   */
  Bool IsTransparent() const;
  /*
   * @brief: This function returns whether window is decorated or not.
   * @return: Whether window is decorated or not.
   */
  Bool IsDecorated() const;
  /*
   * @brief: This function returns whether window is resizable or not.
   * @return: Whether window is resizable or not.
   */
  Bool IsResizable() const;
  /*
   * @brief: This function returns whether window is floating or not.
   * @return: Whether window is floating or not.
   */
  Bool IsFloating() const;
  /*
   * @brief: This function returns whether window is auto iconified or not.
   * @return: Whether window is auto iconified or not.
   * @detail: "Auto iconify" specifies whether the full screen window will
   * automatically iconify and restore the previous video mode on input focus
   * loss.
   */
  Bool IsAutoIconified() const;
  /*
   * @brief: This function returns whether window is focus on showed or not.
   * @return: Whether window is maximized or not.
   * @sa: Show()
   * @detail: "Focus on show" specifies whether the window will be given input
   * focus when Show() function is called.
   */
  Bool IsFocusOnShowed() const;
  /*
   * @brief: This function returns whether key is pressing or not.
   * @return: Whether key is pressing or not.
   */
  Bool IsPressing(Keyboard const &key) const;
  /*
   * @brief: This function returns whether cursor is entering or not.
   * @return: Whether cursor is entering or not.
   */
  Bool IsEntering() const;

  /*
   * @brief: Enable vsync.
   */
  void EnableVsync() const { glfwSwapInterval(1); }
  /*
   * @brief: Disable vsync.
   */
  void DisableVsync() const { glfwSwapInterval(0); }
  /*
   * @brief: This function destroies window.
   */
  void Destroy();
  /*
   * @brief: This function closes window.
   */
  void Close() const;
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
  void Fill(Float const &r, Float const &g, Float const &b) const {
    glClearColor(r, g, b, 0.0f);
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
  void Swap() const;
  /*
   * @brief: This function binds window to current context.
   */
  void Bind() const { this->SetCurrentContext(); }

  operator Bool() const { return !this->IsClosed(); }
};

} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_WINDOW_HPP__
