#pragma once
#include "../common/enum.hpp"
#include "../common/type.hpp"
#include "../util/event.hpp"
#include "../util/property.hpp"

namespace Terreate::Core {
class IWindow;

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
  Type::Keyboard key = Type::Keyboard::K_LAST;
  Type::i32 scancode = 0;
  bool pressed = 0;
  Modifier mods = 0;

public:
  Key(int key_, int scancode_, int action_, int mods_)
      : key((Type::Keyboard)key_), scancode(scancode_), pressed((bool)action_),
        mods(mods_) {}
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

typedef Util::Event<IWindow *, Type::i32 const &, Type::i32 const &>
    WindowPositionEvent;
typedef Util::Event<IWindow *, Type::i32 const &, Type::i32 const &>
    WindowSizeEvent;
typedef Util::Event<IWindow *> WindowCloseEvent;
typedef Util::Event<IWindow *> WindowRefreshEvent;
typedef Util::Event<IWindow *, bool const &> WindowFocusEvent;
typedef Util::Event<IWindow *, bool const &> WindowIconifyEvent;
typedef Util::Event<IWindow *, bool const &> WindowMaximizeEvent;
typedef Util::Event<IWindow *, Type::u32 const &, Type::u32 const &>
    WindowFramebufferSizeEvent;
typedef Util::Event<IWindow *, float const &, float const &>
    WindowContentScaleEvent;
typedef Util::Event<IWindow *, Type::u32 const &, Type::u32 const &,
                    Modifier const &>
    MousebuttonEvent;
typedef Util::Event<IWindow *, double const &, double const &>
    CursorPositionEvent;
typedef Util::Event<IWindow *, bool const &> CursorEnterEvent;
typedef Util::Event<IWindow *, double const &, double const &> ScrollEvent;
typedef Util::Event<IWindow *, Key const &> KeyEvent;
typedef Util::Event<IWindow *, Type::u32 const &> CharEvent;
typedef Util::Event<IWindow *, Type::vec<Type::str> const &> FileDropEvent;

typedef Util::Subscriber<IWindow *, Type::i32 const &, Type::i32 const &>
    WindowPositionSubscriber;
typedef Util::Subscriber<IWindow *, Type::i32 const &, Type::i32 const &>
    WindowSizeSubscriber;
typedef Util::Subscriber<IWindow *> WindowCloseSubscriber;
typedef Util::Subscriber<IWindow *> WindowRefreshSubscriber;
typedef Util::Subscriber<IWindow *, bool const &> WindowFocusSubscriber;
typedef Util::Subscriber<IWindow *, bool const &> WindowIconifySubscriber;
typedef Util::Subscriber<IWindow *, bool const &> WindowMaximizeSubscriber;
typedef Util::Subscriber<IWindow *, Type::u32 const &, Type::u32 const &>
    WindowFramebufferSizeSubscriber;
typedef Util::Subscriber<IWindow *, float const &, float const &>
    WindowContentScaleSubscriber;
typedef Util::Subscriber<IWindow *, Type::u32 const &, Type::u32 const &,
                         Modifier const &>
    MousebuttonSubscriber;
typedef Util::Subscriber<IWindow *, double const &, double const &>
    CursorPositionSubscriber;
typedef Util::Subscriber<IWindow *, bool const &> CursorEnterSubscriber;
typedef Util::Subscriber<IWindow *, double const &, double const &>
    ScrollSubscriber;
typedef Util::Subscriber<IWindow *, Key const &> KeySubscriber;
typedef Util::Subscriber<IWindow *, Type::u32 const &> CharSubscriber;
typedef Util::Subscriber<IWindow *, Type::vec<Type::str> const &>
    FileDropSubscriber;

class WindowProperties {
private:
  // Read-only properties
  Type::pair<Type::i32> mFramebufferSize = {0, 0};
  Type::pair<double> mScrollOffset = {0.0, 0.0};
  Type::vec<Type::u32> mCodePoints;
  Type::vec<Key> mKeys;
  Type::vec<Type::str> mDroppedFiles;

  // Read-write properties
  Type::pair<Type::i32> mSize;
  Type::pair<Type::i32> mPosition;
  Type::pair<double> mCursorPosition;
  Type::str mClipboard;
  Type::str mTitle;
  float mOpacity;

public:
  // Read-only properties
  Util::Property<Type::pair<Type::i32>> framebufferSize;
  Util::Property<Type::pair<double>> scrollOffset;
  Util::Property<Type::vec<Type::u32>> codePoints;
  Util::Property<Type::vec<Key>> keys;
  Util::Property<Type::vec<Type::str>> droppedFiles;

  // Read-write properties
  Util::Property<Type::pair<Type::i32>> size;
  Util::Property<Type::pair<Type::i32>> position;
  Util::Property<Type::pair<double>> cursorPosition;
  Util::Property<Type::str> clipboard;
  Util::Property<Type::str> title;
  Util::Property<float> opacity;

public:
  WindowProperties() = default;
  void setup(GLFWwindow *window);
};

struct WindowEvent {
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
};
} // namespace Terreate::Core
