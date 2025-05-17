// #pragma once

// #include <api.hpp>
// #include <device.hpp>
// #include <enum.hpp>
// #include <event.hpp>
// #include <hubptr.hpp>
// #include <interface.hpp>
// #include <property.hpp>
// #include <swapchain.hpp>
// #include <type.hpp>

// namespace Terreate::Core {
// using namespace Terreate::Type;
// using namespace Terreate::Enum;
// using namespace Terreate::Util::Pointer;

// struct Modifier {
// public:
//   bool shift = false;
//   bool control = false;
//   bool alt = false;
//   bool super = false;
//   bool capsLock = false;
//   bool numLock = false;

// public:
//   Modifier(int mods_);
// };

// struct Key {
// public:
//   Keyboard key = Keyboard::K_LAST;
//   i32 scancode = 0;
//   bool pressed = 0;
//   Modifier mods = 0;

// public:
//   Key(int key_, int scancode_, int action_, int mods_);
// };

// namespace Wrapper {
// void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos);
// void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height);
// void windowCloseCallbackWrapper(GLFWwindow *window);
// void windowRefreshCallbackWrapper(GLFWwindow *window);
// void windowFocusCallbackWrapper(GLFWwindow *window, int focused);
// void windowIconifyCallbackWrapper(GLFWwindow *window, int iconified);
// void windowMaximizeCallbackWrapper(GLFWwindow *window, int maximized);
// void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
//                                           int height);
// void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
//                                        float yscale);
// void mousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
//                                 int mods);
// void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
//                                    double ypos);
// void cursorEnterCallbackWrapper(GLFWwindow *window, int entered);
// void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double
// yoffset); void keyCallbackWrapper(GLFWwindow *window, int key, int scancode,
// int action,
//                         int mods);
// void charCallbackWrapper(GLFWwindow *window, unsigned codepoint);
// void dropCallbackWrapper(GLFWwindow *window, int count, char const **paths);
// } // namespace Wrapper

// class Window;

// typedef Event<Window *, i32 const &, i32 const &> WindowPositionEvent;
// typedef Event<Window *, i32 const &, i32 const &> WindowSizeEvent;
// typedef Event<Window *> WindowCloseEvent;
// typedef Event<Window *> WindowRefreshEvent;
// typedef Event<Window *, bool const &> WindowFocusEvent;
// typedef Event<Window *, bool const &> WindowIconifyEvent;
// typedef Event<Window *, bool const &> WindowMaximizeEvent;
// typedef Event<Window *, u32 const &, u32 const &> WindowFramebufferSizeEvent;
// typedef Event<Window *, float const &, float const &>
// WindowContentScaleEvent; typedef Event<Window *, u32 const &, u32 const &,
// Modifier const &>
//     MousebuttonEvent;
// typedef Event<Window *, double const &, double const &> CursorPositionEvent;
// typedef Event<Window *, bool const &> CursorEnterEvent;
// typedef Event<Window *, double const &, double const &> ScrollEvent;
// typedef Event<Window *, Key const &> KeyEvent;
// typedef Event<Window *, u32 const &> CharEvent;
// typedef Event<Window *, vec<str> const &> FileDropEvent;

// namespace Subscribers {
// typedef Subscriber<Window *, i32 const &, i32 const &>
// WindowPositionSubscriber; typedef Subscriber<Window *, i32 const &, i32 const
// &> WindowSizeSubscriber; typedef Subscriber<Window *> WindowCloseSubscriber;
// typedef Subscriber<Window *> WindowRefreshSubscriber;
// typedef Subscriber<Window *, bool const &> WindowFocusSubscriber;
// typedef Subscriber<Window *, bool const &> WindowIconifySubscriber;
// typedef Subscriber<Window *, bool const &> WindowMaximizeSubscriber;
// typedef Subscriber<Window *, u32 const &, u32 const &>
//     WindowFramebufferSizeSubscriber;
// typedef Subscriber<Window *, float const &, float const &>
//     WindowContentScaleSubscriber;
// typedef Subscriber<Window *, u32 const &, u32 const &, Modifier const &>
//     MousebuttonSubscriber;
// typedef Subscriber<Window *, double const &, double const &>
//     CursorPositionSubscriber;
// typedef Subscriber<Window *, bool const &> CursorEnterSubscriber;
// typedef Subscriber<Window *, double const &, double const &>
// ScrollSubscriber; typedef Subscriber<Window *, Key const &> KeySubscriber;
// typedef Subscriber<Window *, u32 const &> CharSubscriber;
// typedef Subscriber<Window *, vec<str> const &> FileDropSubscriber;
// } // namespace Subscribers

// class Icon final {
// private:
//   vec<GLFWimage> mImages;
//   vec<u8 *> mPointers;

// private:
//   Icon(Icon const &) = delete;
//   Icon &operator=(Icon const &) = delete;

// public:
//   /*
//    * @brief: This function creates a glfw icon.
//    */
//   Icon();
//   ~Icon();

//   /*
//    * @brief: This function returns the number of images in the icon.
//    * @return: The number of images in the icon.
//    */
//   u32 getImageCount() const;

//   /*
//    * @brief: This function adds an image to the icon.
//    * @param: width: Image width.
//    * @param: height: Image height.
//    * @param: pixels: Image pixels.
//    * @detail: Pixels are copied to new allocated array and set to "glfw
//    image"
//    * instance. "glfw image" is used to render icon. Icon can have multiple of
//    * images.
//    */
//   void addImage(u32 const &width, u32 const &height, u8 const *pixels);

//   operator GLFWimage const *() const;
//   operator bool() const;
// };

// class Cursor final {
// private:
//   GLFWcursor *mCursor = nullptr;
//   GLFWimage mImage = GLFWimage();
//   u8 *mPixels = nullptr;
//   int mXHot = 0;
//   int mYHot = 0;

// private:
//   Cursor(Cursor const &) = delete;
//   Cursor &operator=(Cursor const &) = delete;

// public:
//   /*
//    * @brief: This function creates a glfw cursor.
//    * @param: xHot: The x-coordinate of the cursor's hot spot.
//    * @param: yHot: The y-coordinate of the cursor's hot spot.
//    */
//   Cursor(i32 const &xHot = 0, i32 const &yHot = 0);
//   ~Cursor();

//   /*
//    * @brief: This function sets the image of the cursor.
//    * @param: width: Image width.
//    * @param: height: Image height.
//    * @param: pixels: Image pixels.
//    * @detail: The image is copied to new allocated array and set to "glfw
//    * image" instance. "glfw image" is used to render cursor.
//    */
//   void setImage(u32 const &width, u32 const &height, u8 const *pixels);

//   operator GLFWcursor *() const;
//   operator bool() const;
// };

// class StandardCursor {
// private:
//   GLFWcursor *mCursor = nullptr;

// private:
//   StandardCursor(StandardCursor const &) = delete;
//   StandardCursor &operator=(StandardCursor const &) = delete;

// public:
//   /*
//    * @brief: This function creates a glfw standard cursor.
//    * @param: shape: Cursor shape.
//    */
//   StandardCursor(CursorShape const &shape);
//   ~StandardCursor();

//   operator GLFWcursor *() const;
//   operator bool() const;
// };

// struct WindowSettings {
// public:
//   bool resizable = true;
//   bool visible = true;
//   bool decorated = true;
//   bool focused = true;
//   bool autoIconify = true;
//   bool floating = false;
//   bool maximized = false;
//   bool centerCursor = true;
//   bool transparentFramebuffer = false;
//   bool focusOnShow = true;
//   bool scaleToMonitor = false;
// };

// class WindowProperties {
// private:
//   // Read-only properties
//   pair<double> mScrollOffset = {0.0, 0.0};
//   vec<u32> mCodePoints;
//   vec<Key> mKeys;
//   vec<str> mDroppedFiles;

//   // Read-write properties
//   pair<i32> mSize;
//   pair<i32> mPosition;
//   pair<double> mCursorPosition;
//   str mClipboard;
//   str mTitle;
//   float mOpacity;

// public:
//   // Read-only properties
//   Property<pair<double>> scrollOffset;
//   Property<vec<u32>> codePoints;
//   Property<vec<Key>> keys;
//   Property<vec<str>> droppedFiles;

//   // Read-write properties
//   Property<pair<i32>> size;
//   Property<pair<i32>> position;
//   Property<pair<double>> cursorPosition;
//   Property<str> clipboard;
//   Property<str> title;
//   Property<float> opacity;

// public:
//   WindowProperties() {}
//   void setup(GLFWwindow *window);
// };

// struct WindowEvent {
// public:
//   WindowPositionEvent onPositionChange;
//   WindowSizeEvent onSizeChange;
//   WindowCloseEvent onClose;
//   WindowRefreshEvent onRefresh;
//   WindowFocusEvent onFocus;
//   WindowIconifyEvent onIconify;
//   WindowMaximizeEvent onMaximize;
//   WindowFramebufferSizeEvent onFramebufferSizeChange;
//   WindowContentScaleEvent onContentScaleChange;
//   MousebuttonEvent onMousebuttonInput;
//   CursorPositionEvent onCursorPositionChange;
//   CursorEnterEvent onCursorEnter;
//   ScrollEvent onScroll;
//   KeyEvent onKeyInput;
//   CharEvent onCharInput;
//   FileDropEvent onFileDrop;
// };

// class Window : public Interface::IBindable {
// private:
//   PROHIBIT_COPY_AND_ASSIGN(Window);

// private:
//   VkInstance mInstance = VK_NULL_HANDLE;
//   GLFWwindow *mWindow = nullptr;
//   VkSurfaceKHR mSurface = VK_NULL_HANDLE;
//   HubPointer<Swapchain> mSwapchain;

// public:
//   WindowEvent events;
//   WindowProperties properties;

// private:
//   void setWindowHints(WindowSettings const &settings);
//   void setEventCallbacks();

// public:
//   Window(VkInstance instance, str const &title, int width, int height,
//          WindowSettings const &settings);
//   ~Window();

//   bool getMousebutton(MousebuttonInput const &button) const;
//   bool getInputState(InputType const &type) const;

//   void setIcon(Icon const &icon);
//   void setCursor(Cursor const &cursor);
//   void setCursor(StandardCursor const &cursor);
//   void setDefaultCursor();

//   LogicalDevice *createDevice() const;
//   void createSwapchain(RefPointer<LogicalDevice> device);

//   void bind() override;
//   void unbind() override;
// };

// } // namespace Terreate::Core
