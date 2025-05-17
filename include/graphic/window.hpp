#ifndef __TERREATE_GRAPHICS_WINDOW_HPP__
#define __TERREATE_GRAPHICS_WINDOW_HPP__

#include <core/property.hpp>
#include <core/type.hpp>
#include <graphic/GLdefs.hpp>
#include <io/image.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;
using namespace Terreate::Core;

struct Modifier {
public:
  bool shift = false;
  bool control = false;
  bool alt = false;
  bool super = false;
  bool capsLock = false;
  bool numLock = false;

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
  i32 scancode = 0;
  bool pressed = 0;
  Modifier mods = 0;

public:
  Key(int key_, int scancode_, int action_, int mods_)
      : key((Keyboard)key_), scancode(scancode_), pressed((bool)action_),
        mods(mods_) {}
};

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
typedef Event<Window *, u32 const &, u32 const &, Modifier const &>
    MousebuttonEvent;
typedef Event<Window *, double const &, double const &> CursorPositionEvent;
typedef Event<Window *, bool const &> CursorEnterEvent;
typedef Event<Window *, double const &, double const &> ScrollEvent;
typedef Event<Window *, Key const &> KeyEvent;
typedef Event<Window *, u32 const &> CharEvent;
typedef Event<Window *, Vec<Str> const &> FileDropEvent;

namespace Subscribers {
typedef Subscriber<Window *, i32 const &, i32 const &> WindowPositionSubscriber;
typedef Subscriber<Window *, i32 const &, i32 const &> WindowSizeSubscriber;
typedef Subscriber<Window *> WindowCloseSubscriber;
typedef Subscriber<Window *> WindowRefreshSubscriber;
typedef Subscriber<Window *, bool const &> WindowFocusSubscriber;
typedef Subscriber<Window *, bool const &> WindowIconifySubscriber;
typedef Subscriber<Window *, bool const &> WindowMaximizeSubscriber;
typedef Subscriber<Window *, u32 const &, u32 const &>
    WindowFramebufferSizeSubscriber;
typedef Subscriber<Window *, float const &, float const &>
    WindowContentScaleSubscriber;
typedef Subscriber<Window *, u32 const &, u32 const &, Modifier const &>
    MousebuttonSubscriber;
typedef Subscriber<Window *, double const &, double const &>
    CursorPositionSubscriber;
typedef Subscriber<Window *, bool const &> CursorEnterSubscriber;
typedef Subscriber<Window *, double const &, double const &> ScrollSubscriber;
typedef Subscriber<Window *, Key const &> KeySubscriber;
typedef Subscriber<Window *, u32 const &> CharSubscriber;
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

  u32 getSize() const { return mImages.size(); }

  /*
   * @brief: This function adds an image to the icon.
   * @param: width: Image width.
   * @param: height: Image height.
   * @param: pixels: Image pixels.
   * @detail: Pixels are copied to new allocated array and set to "glfw image"
   * instance. "glfw image" is used to render icon. Icon can have multiple of
   * images.
   */
  void addImage(u32 const &width, u32 const &height, Ubyte const *pixels);
  /*
   * @brief: This function adds an image to the icon.
   * @param: texture: Image texture.
   * @detail: Pixels are copied to new allocated array and set to "glfw image"
   * instance. "glfw image" is used to render icon. Icon can have multiple of
   * images.
   */
  void addImage(IO::Texture const &texture) {
    this->addImage(texture.width, texture.height, texture.data.data());
  }

  operator GLFWimage const *() const { return mImages.data(); }
  operator bool() const { return mImages.size() > 0; }
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
  Cursor(i32 const &xHot = 0, i32 const &yHot = 0) : mXHot(xHot), mYHot(yHot) {}
  ~Cursor();

  /*
   * @brief: This function sets the image of the cursor.
   * @param: width: Image width.
   * @param: height: Image height.
   * @param: pixels: Image pixels.
   * @detail: The image is copied to new allocated array and set to "glfw
   * image" instance. "glfw image" is used to render cursor.
   */
  void setImage(u32 const &width, u32 const &height, Ubyte const *pixels);

  operator GLFWcursor *() const { return mCursor; }
  operator bool() const { return mCursor != nullptr; }
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
    mCursor = glfwCreateStandardCursor((u32)shape);
  }
  ~StandardCursor() { glfwDestroyCursor(mCursor); }

  operator GLFWcursor *() const { return mCursor; }
  operator bool() const { return mCursor != nullptr; }
};

namespace Callbacks {
void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos);
void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height);
void windowCloseCallbackWrapper(GLFWwindow *window);
void windowRefreshCallbackWrapper(GLFWwindow *window);
void windowFocusCallbackWrapper(GLFWwindow *window, int focused);
void windowIconifyCallbackWrapper(GLFWwindow *window, int iconified);
void windowMaximizeCallbackWrapper(GLFWwindow *window, int maximized);
void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                          int height);
void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale);
void mousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
                                int mods);
void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos);
void cursorEnterCallbackWrapper(GLFWwindow *window, int entered);
void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset);
void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods);
void charCallbackWrapper(GLFWwindow *window, u32 codepoint);
void dropCallbackWrapper(GLFWwindow *window, int count, const char **paths);
} // namespace Callbacks

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

class WindowProperty {
private:
  WindowProperty(WindowProperty const &) = delete;
  WindowProperty &operator=(WindowProperty const &) = delete;

private:
  // Read-only properties
  Pair<double> mScrollOffset;
  Vec<u32> mCodePoints;
  Vec<Key> mKeys;
  Vec<Str> mDroppedFiles;

  // Read-write properties
  Pair<i32> mSize;
  Pair<i32> mPosition;
  Pair<double> mCursorPosition;
  Str mClipboard;
  Str mTitle;
  float mOpacity;

public:
  // Read-only properties
  Property<Pair<double>> scrollOffset;
  Property<Vec<u32>> codePoints;
  Property<Vec<Key>> keys;
  Property<Vec<Str>> droppedFiles;
  Property<CursorMode> cursorMode;

  // Read-write properties
  Property<Pair<i32>> size;
  Property<Pair<i32>> position;
  Property<Pair<double>> cursorPosition;
  Property<Str> clipboard;
  Property<Str> title;
  Property<float> opacity;

private:
  friend class Window;
  void setup(GLFWwindow *window);

public:
  WindowProperty() = default;
  ~WindowProperty() = default;
};

class WindowEvent {
private:
  WindowEvent(WindowEvent const &) = delete;
  WindowEvent &operator=(WindowEvent const &) = delete;

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
  MousebuttonEvent onMousebuttonInput;
  CursorPositionEvent onCursorPositionChange;
  CursorEnterEvent onCursorEnter;
  ScrollEvent onScroll;
  KeyEvent onKeyInput;
  CharEvent onCharInput;
  FileDropEvent onFileDrop;

public:
  WindowEvent() = default;
  ~WindowEvent() = default;
};

class Window {
private:
  Window(Window const &) = delete;
  Window &operator=(Window const &) = delete;

private:
  GLFWwindow *mWindow = nullptr;

public:
  WindowProperty property;
  WindowEvent event;

public:
  /*
   * @brief: This function creates a glfw window and sets the callbacks.
   * @param: width: Window width.
   * @param: height: Window height.
   * @param: title: Window title.
   * @param: settings: Window settings.
   * @param: shared: GL context to share resources with.
   */
  Window(u32 const &width, u32 const &height, Str const &title,
         WindowSettings const &settings, Window *shared = nullptr);
  ~Window() { this->destroy(); }

  /*
   * @brief: This function returns specified mouse button state.
   * @return: Mousebutton state [true(Pressed)/false(Released)].
   */
  bool getMousebutton(MousebuttonInput const &button) const {
    return bool(glfwGetMouseButton(mWindow, (u32)button) == GLFW_PRESS);
  }
  /*
   * @brief: This function returns specified input type state.
   * @return: Input type state. [true(enabled)/false(disabled)]
   * @note: If you want to get cursor mode, use GetCursorMode() function.
   */
  bool getInputTypeState(InputType const &type) const {
    return bool(glfwGetInputMode(mWindow, (u32)type) == GLFW_TRUE);
  }

  /*
   * @brief: This function sets window as current context.
   */
  void setCurrent() const { glfwMakeContextCurrent(mWindow); }
  /*
   * @brief: This function sets window icon.
   * @param: icon: Window icon.
   */
  void setIcon(Icon const &icon) {
    glfwSetWindowIcon(mWindow, icon.getSize(), (GLFWimage const *)icon);
  }
  /*
   * @brief: This function sets cursor appearance.
   * @param: cursor: Cursor data.
   */
  void setCursor(Cursor const &cursor) {
    glfwSetCursor(mWindow, (GLFWcursor *)cursor);
  }
  /*
   * @brief: This function sets standard cursor appearance.
   * @param: cursor: Standard cursor data.
   */
  void setCursor(StandardCursor const &cursor) {
    glfwSetCursor(mWindow, (GLFWcursor *)cursor);
  }
  /*
   * @brief: This function sets window cursor to default.
   */
  void setDefaultCursor() { glfwSetCursor(mWindow, nullptr); }
  /*
   * @brief: This function sets specified input type state.
   * @param: type: Input type.
   * @param: state: Input type state.
   */
  void setInputTypeState(InputType const &type, bool const &state) {
    glfwSetInputMode(mWindow, (int)type, state);
  }
  /*
   * @brief: This function sets window view port.
   * @param: x0: View port x0.
   * @param: y0: View port y0.
   * @param: width: View port width.
   * @param: height: View port height.
   */
  void setViewPort(u32 const &x0, u32 const &y0, u32 const &width,
                   u32 const &height) {
    glViewport(x0, y0, width, height);
  }
  /*
   * @brief: This function sets window view port.
   * @param: width: View port width.
   * @param: height: View port height.
   */
  void setViewPort(u32 const &width, u32 const &height) {
    glViewport(0, 0, width, height);
  }

  /*
   * @brief: This function returns whether window is closed or not.
   * @return: Whether window is closed or not.
   */
  bool isClosed() const;
  /*
   * @brief: This function returns whether window is fullscreen or not.
   * @return: Whether window is fullscreen or not.
   */
  bool isFullScreen() const;
  /*
   * @brief: This function returns whether window is windowed or not.
   * @return: Whether window is windowed or not.
   */
  bool isWindowed() const;
  /*
   * @brief: This function returns whether window is iconified or not.
   * @return: Whether window is iconified or not.
   */
  bool isIconified() const;
  /*
   * @brief: This function returns whether window is maximized or not.
   * @return: Whether window is maximized or not.
   */
  bool isMaximized() const;
  /*
   * @brief: This function returns whether window is visible or not.
   * @return: Whether window is visible or not.
   */
  bool isVisible() const;
  /*
   * @brief: This function returns whether window is focused or not.
   * @return: Whether window is focused or not.
   */
  bool isFocused() const;
  /*
   * @brief: This function returns whether window is transparent or not.
   * @return: Whether window is transparent or not.
   */
  bool isTransparent() const;
  /*
   * @brief: This function returns whether window is decorated or not.
   * @return: Whether window is decorated or not.
   */
  bool isDecorated() const;
  /*
   * @brief: This function returns whether window is resizable or not.
   * @return: Whether window is resizable or not.
   */
  bool isResizable() const;
  /*
   * @brief: This function returns whether window is floating or not.
   * @return: Whether window is floating or not.
   */
  bool isFloating() const;
  /*
   * @brief: This function returns whether window is auto iconified or not.
   * @return: Whether window is auto iconified or not.
   * @detail: "Auto iconify" specifies whether the full screen window will
   * automatically iconify and restore the previous video mode on input focus
   * loss.
   */
  bool isAutoIconified() const;
  /*
   * @brief: This function returns whether window is focus on showed or not.
   * @return: Whether window is maximized or not.
   * @sa: Show()
   * @detail: "Focus on show" specifies whether the window will be given input
   * focus when Show() function is called.
   */
  bool isFocusOnShowed() const;
  /*
   * @brief: This function returns whether key is pressing or not.
   * @return: Whether key is pressing or not.
   */
  bool isPressing(Keyboard const &key) const;
  /*
   * @brief: This function returns whether cursor is entering or not.
   * @return: Whether cursor is entering or not.
   */
  bool isEntering() const;

  /*
   * @brief: Enable vsync.
   */
  void enableVsync() const { glfwSwapInterval(1); }
  /*
   * @brief: Disable vsync.
   */
  void disableVsync() const { glfwSwapInterval(0); }
  /*
   * @brief: This function destroies window.
   */
  void destroy();
  /*
   * @brief: This function closes window.
   */
  void close() const;
  /*
   * @brief: This function iconifies window.
   */
  void iconify() const;
  /*
   * @brief: This function maximizes window.
   */
  void maximize() const;
  /*
   * @brief: This function shows window.
   */
  void show() const;
  /*
   * @brief: This function hides window.
   */
  void hide() const;
  /*
   * @brief: This function focuses window.
   */
  void focus() const;
  /*
   * @brief: This function unmaximize or uniconify window.
   */
  void restore() const;
  /*
   * @brief: This function requests attention to window.
   */
  void requestAttention() const;
  /*
   * @brief: This function fills window with specified color.
   * @param: color: Fill color.
   * @detail: Color format is (red, green, blue). Each color is float (0
   * ~ 1.0).
   */
  void fill(float const &r, float const &g, float const &b) const {
    glClearColor(r, g, b, 0.0f);
  }
  /*
   * @brief: This function clears color, depth, and stencil buffers.
   */
  void clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }
  /*
   * @brief: This function clears color buffer.
   */
  void clearColor() const { glClear(GL_COLOR_BUFFER_BIT); }
  /*
   * @brief: This function clears depth buffer.
   */
  void clearDepth() const { glClear(GL_DEPTH_BUFFER_BIT); }
  /*
   * @brief: This function clears stencil buffer.
   */
  void clearStencil() const { glClear(GL_STENCIL_BUFFER_BIT); }
  /*
   * @brief: This function swaps front and back buffers.
   */
  void swap() const;
  /*
   * @brief: This function binds window to current context.
   */
  void bind() const { this->setCurrent(); }
  /*
   * @brief: This function unbinds window from current context.
   */
  void unbind() const { glfwMakeContextCurrent(nullptr); }

  operator bool() const { return !this->isClosed(); }
};

} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_WINDOW_HPP__
