// #include <exception.hpp>
// #include <window.hpp>

// namespace Terreate::Core {
// Modifier::Modifier(int mods_)
//     : shift(mods_ & GLFW_MOD_SHIFT), control(mods_ & GLFW_MOD_CONTROL),
//       alt(mods_ & GLFW_MOD_ALT), super(mods_ & GLFW_MOD_SUPER),
//       capsLock(mods_ & GLFW_MOD_CAPS_LOCK), numLock(mods_ &
//       GLFW_MOD_NUM_LOCK) {
// }

// Key::Key(int key_, int scancode_, int action_, int mods_)
//     : key((Keyboard)key_), scancode(scancode_), pressed((bool)action_),
//       mods(mods_) {}

// namespace Wrappers {
// void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->properties.position = pair<i32>(xpos, ypos);
//   ptr->events.onPositionChange.publish(ptr, xpos, ypos);
// }

// void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->properties.size = pair<u32>(width, height);
//   ptr->events.onSizeChange.publish(ptr, width, height);
// }

// void windowCloseCallbackWrapper(GLFWwindow *window) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onClose.publish(ptr);
// }

// void windowRefreshCallbackWrapper(GLFWwindow *window) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onRefresh.publish(ptr);
// }

// void windowFocusCallbackWrapper(GLFWwindow *window, int focused) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onFocus.publish(ptr, (bool)focused);
// }

// void windowIconifyCallbackWrapper(GLFWwindow *window, int iconified) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onIconify.publish(ptr, (bool)iconified);
// }

// void windowMaximizeCallbackWrapper(GLFWwindow *window, int maximized) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onMaximize.publish(ptr, (bool)maximized);
// }

// void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
//                                           int height) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onFramebufferSizeChange.publish(ptr, width, height);
// }

// void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
//                                        float yscale) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onContentScaleChange.publish(ptr, xscale, yscale);
// }

// void mousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
//                                 int mods) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onMousebuttonInput.publish(ptr, button, action,
//   Modifier(mods));
// }

// void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
//                                    double ypos) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->properties.cursorPosition = pair<double>(xpos, ypos);
//   ptr->events.onCursorPositionChange.publish(ptr, xpos, ypos);
// }

// void cursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->events.onCursorEnter.publish(ptr, (bool)entered);
// }

// void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double
// yoffset) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->properties.scrollOffset = pair<double>(xoffset, yoffset);
//   ptr->events.onScroll.publish(ptr, xoffset, yoffset);
// }

// void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int
// action,
//                         int mods) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   Key wrappedKey = Key(key, scancode, action, mods);
//   ptr->properties.keys.get().push_back(wrappedKey);
//   ptr->events.onKeyInput.publish(ptr, wrappedKey);
// }

// void charCallbackWrapper(GLFWwindow *window, u32 codepoint) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   ptr->properties.codePoints.get().push_back(codepoint);
//   ptr->events.onCharInput.publish(ptr, codepoint);
// }

// void dropCallbackWrapper(GLFWwindow *window, int count, const char **paths) {
//   Window *ptr = (Window *)glfwGetWindowUserPointer(window);
//   vec<str> droppedFiles(paths, paths + count);
//   ptr->properties.droppedFiles = droppedFiles;
//   ptr->events.onFileDrop.publish(ptr, droppedFiles);
// }
// } // namespace Wrappers

// Icon::Icon() {
//   mImages = vec<GLFWimage>();
//   mPointers = vec<u8 *>();
// }

// Icon::~Icon() {
//   for (auto &pointer : mPointers) {
//     delete[] pointer;
//   }
// }

// u32 Icon::getImageCount() const { return mImages.size(); }

// Icon::operator GLFWimage const *() const { return mImages.data(); }

// Icon::operator bool() const { return mImages.size() > 0; }

// void Icon::addImage(u32 const &width, u32 const &height, u8 const *pixels) {
//   GLFWimage image;
//   image.width = width;
//   image.height = height;
//   u8 *newPixels = new u8[width * height * 4];
//   memcpy(newPixels, pixels, width * height * 4);
//   image.pixels = newPixels;
//   mPointers.push_back(newPixels);
//   mImages.push_back(image);
// }

// Cursor::Cursor(i32 const &xHot, i32 const &yHot) : mXHot(xHot), mYHot(yHot) {
//   mPixels = nullptr;
//   mCursor = nullptr;
// }

// Cursor::~Cursor() {
//   glfwDestroyCursor(mCursor);
//   delete[] mPixels;
// }

// void Cursor::setImage(u32 const &width, u32 const &height, u8 const *pixels)
// {
//   GLFWimage image;
//   image.width = width;
//   image.height = height;
//   mPixels = new u8[width * height * 4];
//   memcpy(mPixels, pixels, width * height * 4);
//   image.pixels = mPixels;
//   mCursor = glfwCreateCursor(&image, mXHot, mYHot);
// }

// Cursor::operator GLFWcursor *() const { return mCursor; }

// Cursor::operator bool() const { return mCursor != nullptr; }

// StandardCursor::StandardCursor(CursorShape const &shape) {
//   if (shape == CursorShape::DEFAULT) {
//     mCursor = nullptr;
//   } else {
//     mCursor = glfwCreateStandardCursor((u32)shape);
//   }
// }

// StandardCursor::~StandardCursor() {
//   if (mCursor != nullptr) {
//     glfwDestroyCursor(mCursor);
//   }
// }

// StandardCursor::operator GLFWcursor *() const { return mCursor; }

// StandardCursor::operator bool() const { return mCursor != nullptr; }

// void WindowProperties::setup(GLFWwindow *window) {
//   this->scrollOffset.set(&mScrollOffset);
//   this->codePoints.set(&mCodePoints);
//   this->keys.set(&mKeys);
//   this->droppedFiles.set(&mDroppedFiles);

//   this->size.set(&mSize);
//   this->position.set(&mPosition);
//   this->cursorPosition.set(&mCursorPosition);
//   this->clipboard.set(&mClipboard);
//   this->title.set(&mTitle);
//   this->opacity.set(&mOpacity);

//   this->size.getter = [window](pair<i32> *size) -> pair<i32> & {
//     glfwGetWindowSize(window, &(*size).first, &(*size).second);
//     return *size;
//   };
//   this->size.setter = [window](pair<i32> *value, pair<i32> const &size) {
//     glfwSetWindowSize(window, size.first, size.second);
//     *value = size;
//   };
//   this->position.getter = [window](pair<i32> *position) -> pair<i32> & {
//     glfwGetWindowPos(window, &(*position).first, &(*position).second);
//     return *position;
//   };
//   this->position.setter = [window](pair<i32> *value,
//                                    pair<i32> const &position) {
//     glfwSetWindowPos(window, position.first, position.second);
//     *value = position;
//   };
//   this->cursorPosition.getter =
//       [window](pair<double> *position) -> pair<double> & {
//     glfwGetCursorPos(window, &(*position).first, &(*position).second);
//     return *position;
//   };
//   this->cursorPosition.setter = [window](pair<double> *value,
//                                          pair<double> const &position) {
//     glfwSetCursorPos(window, position.first, position.second);
//     *value = position;
//   };
//   this->clipboard.getter = [window](str *clipboard) -> str & {
//     char const *clipboardString = glfwGetClipboardString(window);
//     if (clipboardString) {
//       *clipboard = clipboardString;
//     } else {
//       *clipboard = "";
//     }
//     return *clipboard;
//   };
//   this->clipboard.setter = [window](str *value, str const &clipboard) {
//     glfwSetClipboardString(window, clipboard.c_str());
//     *value = clipboard;
//   };
//   this->title.getter = [window](str *title) -> str & {
//     char const *titleString = glfwGetWindowTitle(window);
//     if (titleString) {
//       *title = titleString;
//     } else {
//       *title = "";
//     }
//     return *title;
//   };
//   this->title.setter = [window](str *value, str const &title) {
//     glfwSetWindowTitle(window, title.c_str());
//     *value = title;
//   };
//   this->opacity.getter = [window](float *opacity) -> float & {
//     *opacity = glfwGetWindowOpacity(window);
//     return *opacity;
//   };
//   this->opacity.setter = [window](float *value, float const &opacity) {
//     glfwSetWindowOpacity(window, opacity);
//     *value = opacity;
//   };
// }

// void Window::setWindowHints(WindowSettings const &settings) {
//   glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
//   glfwWindowHint(GLFW_VISIBLE, settings.visible);
//   glfwWindowHint(GLFW_DECORATED, settings.decorated);
//   glfwWindowHint(GLFW_FOCUSED, settings.focused);
//   glfwWindowHint(GLFW_AUTO_ICONIFY, settings.autoIconify);
//   glfwWindowHint(GLFW_FLOATING, settings.floating);
//   glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);
//   glfwWindowHint(GLFW_CENTER_CURSOR, settings.centerCursor);
//   glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
//   settings.transparentFramebuffer); glfwWindowHint(GLFW_FOCUS_ON_SHOW,
//   settings.focusOnShow); glfwWindowHint(GLFW_SCALE_TO_MONITOR,
//   settings.scaleToMonitor); glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
// }

// void Window::setEventCallbacks() {
//   this->properties.setup(mWindow);

//   using namespace Wrappers;
//   glfwSetWindowPosCallback(mWindow, windowPositionCallbackWrapper);
//   glfwSetWindowSizeCallback(mWindow, windowSizeCallbackWrapper);
//   glfwSetWindowCloseCallback(mWindow, windowCloseCallbackWrapper);
//   glfwSetWindowRefreshCallback(mWindow, windowRefreshCallbackWrapper);
//   glfwSetWindowFocusCallback(mWindow, windowFocusCallbackWrapper);
//   glfwSetWindowIconifyCallback(mWindow, windowIconifyCallbackWrapper);
//   glfwSetWindowMaximizeCallback(mWindow, windowMaximizeCallbackWrapper);
//   glfwSetFramebufferSizeCallback(mWindow,
//   windowFramebufferSizeCallbackWrapper);
//   glfwSetWindowContentScaleCallback(mWindow,
//   windowContentScaleCallbackWrapper); glfwSetMouseButtonCallback(mWindow,
//   mousebuttonCallbackWrapper); glfwSetCursorPosCallback(mWindow,
//   cursorPositionCallbackWrapper); glfwSetCursorEnterCallback(mWindow,
//   cursorEnterCallbackWrapper); glfwSetScrollCallback(mWindow,
//   scrollCallbackWrapper); glfwSetKeyCallback(mWindow, keyCallbackWrapper);
//   glfwSetCharCallback(mWindow, charCallbackWrapper);
//   glfwSetDropCallback(mWindow, dropCallbackWrapper);
// }

// Window::Window(VkInstance instance, str const &title, int width, int height,
//                WindowSettings const &settings)
//     : mInstance(instance) {
//   this->setWindowHints(settings);
//   mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
//   if (mWindow == nullptr) {
//     throw Exception::WindowCreationFailure();
//   }

//   glfwSetWindowUserPointer(mWindow, this);
//   glfwMakeContextCurrent(mWindow);

//   this->setEventCallbacks();

//   if (glfwCreateWindowSurface(mInstance, mWindow, nullptr, &mSurface) !=
//       VK_SUCCESS) {
//     throw Exception::SurfaceCreationFailure();
//   }
// }

// Window::~Window() {
//   if (mSurface != VK_NULL_HANDLE) {
//     vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
//   }
//   if (mWindow != nullptr) {
//     glfwDestroyWindow(mWindow);
//   }
// }

// bool Window::getMousebutton(MousebuttonInput const &button) const {
//   return glfwGetMouseButton(mWindow, (int)button) == GLFW_PRESS;
// }

// bool Window::getInputState(InputType const &type) const {
//   return glfwGetInputMode(mWindow, (int)type) == GLFW_TRUE;
// }

// void Window::setIcon(Icon const &icon) {
//   glfwSetWindowIcon(mWindow, icon.getImageCount(), icon);
// }

// void Window::setCursor(Cursor const &cursor) { glfwSetCursor(mWindow,
// cursor); }

// void Window::setCursor(StandardCursor const &cursor) {
//   glfwSetCursor(mWindow, cursor);
// }

// LogicalDevice *Window::createDevice() const {
//   return new LogicalDevice(mInstance, mSurface);
// }

// void Window::createSwapchain(RefPointer<LogicalDevice> device) {
//   mSwapchain = HubPointer<Swapchain>::create(mWindow, mSurface, device);
// }

// void Window::bind() { glfwMakeContextCurrent(mWindow); }

// void Window::unbind() { glfwMakeContextCurrent(nullptr); }
// } // namespace Terreate::Core
