#pragma once
#include <core/api.hpp>
#include <core/enum.hpp>
#include <event.hpp>
#include <property.hpp>
#include <type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

class Window;

struct Modifier {
public:
  bool shift = false;
  bool control = false;
  bool alt = false;
  bool super = false;
  bool capsLock = false;
  bool numLock = false;

public:
  Modifier(int mods_);
};

struct Key {
public:
  Keyboard key = Keyboard::K_LAST;
  i32 scancode = 0;
  bool pressed = 0;
  Modifier mods = 0;

public:
  Key(int key_, int scancode_, int action_, int mods_);
};

namespace Wrapper {
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
void charCallbackWrapper(GLFWwindow *window, unsigned codepoint);
void dropCallbackWrapper(GLFWwindow *window, int count, char const **paths);
} // namespace Wrapper

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
typedef Event<Window *, vec<str> const &> FileDropEvent;

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
typedef Subscriber<Window *, vec<str> const &> FileDropSubscriber;

class WindowProperties {
private:
  // Read-only properties
  pair<double> mScrollOffset = {0.0, 0.0};
  vec<u32> mCodePoints;
  vec<Key> mKeys;
  vec<str> mDroppedFiles;

  // Read-write properties
  pair<i32> mSize;
  pair<i32> mPosition;
  pair<double> mCursorPosition;
  str mClipboard;
  str mTitle;
  float mOpacity;

public:
  // Read-only properties
  Property<pair<double>> scrollOffset;
  Property<vec<u32>> codePoints;
  Property<vec<Key>> keys;
  Property<vec<str>> droppedFiles;

  // Read-write properties
  Property<pair<i32>> size;
  Property<pair<i32>> position;
  Property<pair<double>> cursorPosition;
  Property<str> clipboard;
  Property<str> title;
  Property<float> opacity;

public:
  WindowProperties();
  void setup(GLFWwindow *window);
};

class WindowEvent {
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
  WindowEvent();
};
} // namespace Terreate::Core
