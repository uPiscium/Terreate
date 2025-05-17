#include <core/exception.hpp>
#include <graphic/window.hpp>

namespace Terreate::Graphic {
Icon::Icon() {
  mImages = Vec<GLFWimage>();
  mPointers = Vec<Ubyte *>();
}

Icon::~Icon() {
  for (auto &pointer : mPointers) {
    delete[] pointer;
  }
}

void Icon::addImage(u32 const &width, u32 const &height, Ubyte const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  Ubyte *newPixels = new Ubyte[width * height * 4];
  memcpy(newPixels, pixels, width * height * 4);
  image.pixels = newPixels;
  mPointers.push_back(newPixels);
  mImages.push_back(image);
}

Cursor::~Cursor() {
  glfwDestroyCursor(mCursor);
  delete[] mPixels;
}

void Cursor::setImage(u32 const &width, u32 const &height,
                      Ubyte const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  mPixels = new Ubyte[width * height * 4];
  memcpy(mPixels, pixels, width * height * 4);
  image.pixels = mPixels;
  mCursor = glfwCreateCursor(&image, mXHot, mYHot);
}

namespace Callbacks {
void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->property.position = Pair<i32>(xpos, ypos);
  ptr->event.onPositionChange.publish(ptr, xpos, ypos);
}

void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->property.size = Pair<u32>(width, height);
  ptr->event.onSizeChange.publish(ptr, width, height);
}

void windowCloseCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onClose.publish(ptr);
}

void windowRefreshCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onRefresh.publish(ptr);
}

void windowFocusCallbackWrapper(GLFWwindow *window, int focused) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onFocus.publish(ptr, (bool)focused);
}

void windowIconifyCallbackWrapper(GLFWwindow *window, int iconified) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onIconify.publish(ptr, (bool)iconified);
}

void windowMaximizeCallbackWrapper(GLFWwindow *window, int maximized) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onMaximize.publish(ptr, (bool)maximized);
}

void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                          int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onFramebufferSizeChange.publish(ptr, width, height);
}

void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onContentScaleChange.publish(ptr, xscale, yscale);
}

void mousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
                                int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onMousebuttonInput.publish(ptr, button, action, Modifier(mods));
}

void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->property.cursorPosition = Pair<double>(xpos, ypos);
  ptr->event.onCursorPositionChange.publish(ptr, xpos, ypos);
}

void cursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->event.onCursorEnter.publish(ptr, (bool)entered);
}

void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->property.scrollOffset = Pair<double>(xoffset, yoffset);
  ptr->event.onScroll.publish(ptr, xoffset, yoffset);
}

void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Key wrappedKey = Key(key, scancode, action, mods);
  ptr->property.keys->push_back(wrappedKey);
  ptr->event.onKeyInput.publish(ptr, wrappedKey);
}

void charCallbackWrapper(GLFWwindow *window, u32 codepoint) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->property.codePoints->push_back(codepoint);
  ptr->event.onCharInput.publish(ptr, codepoint);
}

void dropCallbackWrapper(GLFWwindow *window, int count, const char **paths) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Vec<Str> droppedFiles(paths, paths + count);
  ptr->property.droppedFiles = droppedFiles;
  ptr->event.onFileDrop.publish(ptr, droppedFiles);
}
} // namespace Callbacks

void WindowProperty::setup(GLFWwindow *window) {
  this->scrollOffset.set(&mScrollOffset);
  this->codePoints.set(&mCodePoints);
  this->keys.set(&mKeys);
  this->droppedFiles.set(&mDroppedFiles);

  this->size.set(&mSize);
  this->position.set(&mPosition);
  this->cursorPosition.set(&mCursorPosition);
  this->clipboard.set(&mClipboard);
  this->title.set(&mTitle);
  this->opacity.set(&mOpacity);

  this->size.getter = [this, window]() {
    glfwGetWindowSize(window, &mSize.first, &mSize.second);
    return mSize;
  };
  this->size.setter = [this, window](Pair<u32> const &size) {
    glfwSetWindowSize(window, size.first, size.second);
    mSize = size;
  };
  this->position.getter = [this, window]() {
    glfwGetWindowPos(window, &mPosition.first, &mPosition.second);
    return mPosition;
  };
  this->position.setter = [this, window](Pair<i32> const &position) {
    glfwSetWindowPos(window, position.first, position.second);
    mPosition = position;
  };
  this->cursorPosition.getter = [this, window]() {
    glfwGetCursorPos(window, &mCursorPosition.first, &mCursorPosition.second);
    return mCursorPosition;
  };
  this->cursorPosition.setter = [this, window](Pair<double> const &position) {
    glfwSetCursorPos(window, position.first, position.second);
    mCursorPosition = position;
  };
  this->clipboard.getter = [this, window]() {
    mClipboard = glfwGetClipboardString(NULL);
    return mClipboard;
  };
  this->clipboard.setter = [this, window](Str const &text) {
    glfwSetClipboardString(window, text.c_str());
    mClipboard = text;
  };
  this->title.getter = [this, window]() {
    mTitle = glfwGetWindowTitle(window);
    return mTitle;
  };
  this->title.setter = [this, window](Str const &title) {
    glfwSetWindowTitle(window, title.c_str());
    mTitle = title;
  };
  this->opacity.getter = [this, window]() {
    mOpacity = glfwGetWindowOpacity(window);
    return mOpacity;
  };
  this->opacity.setter = [this, window](float const &opacity) {
    glfwSetWindowOpacity(window, opacity);
    mOpacity = opacity;
  };
}

Window::Window(u32 const &width, u32 const &height, Str const &title,
               WindowSettings const &settings, Window *shared) {
  glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
  glfwWindowHint(GLFW_VISIBLE, settings.visible);
  glfwWindowHint(GLFW_DECORATED, settings.decorated);
  glfwWindowHint(GLFW_FOCUSED, settings.focused);
  glfwWindowHint(GLFW_AUTO_ICONIFY, settings.autoIconify);
  glfwWindowHint(GLFW_FLOATING, settings.floating);
  glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);
  glfwWindowHint(GLFW_CENTER_CURSOR, settings.centerCursor);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, settings.transparentFramebuffer);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, settings.focusOnShow);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, settings.scaleToMonitor);

  GLFWwindow *sharedWindow = shared ? shared->mWindow : nullptr;
  mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, sharedWindow);
  glfwSetWindowUserPointer(mWindow, this);
  glfwMakeContextCurrent(mWindow);

  this->property.setup(mWindow);

  glfwSetWindowPosCallback(mWindow, Callbacks::windowPositionCallbackWrapper);
  glfwSetWindowSizeCallback(mWindow, Callbacks::windowSizeCallbackWrapper);
  glfwSetWindowCloseCallback(mWindow, Callbacks::windowCloseCallbackWrapper);
  glfwSetWindowRefreshCallback(mWindow,
                               Callbacks::windowRefreshCallbackWrapper);
  glfwSetWindowFocusCallback(mWindow, Callbacks::windowFocusCallbackWrapper);
  glfwSetWindowIconifyCallback(mWindow,
                               Callbacks::windowIconifyCallbackWrapper);
  glfwSetWindowMaximizeCallback(mWindow,
                                Callbacks::windowMaximizeCallbackWrapper);
  glfwSetFramebufferSizeCallback(
      mWindow, Callbacks::windowFramebufferSizeCallbackWrapper);
  glfwSetWindowContentScaleCallback(
      mWindow, Callbacks::windowContentScaleCallbackWrapper);
  glfwSetMouseButtonCallback(mWindow, Callbacks::mousebuttonCallbackWrapper);
  glfwSetCursorPosCallback(mWindow, Callbacks::cursorPositionCallbackWrapper);
  glfwSetCursorEnterCallback(mWindow, Callbacks::cursorEnterCallbackWrapper);
  glfwSetScrollCallback(mWindow, Callbacks::scrollCallbackWrapper);
  glfwSetKeyCallback(mWindow, Callbacks::keyCallbackWrapper);
  glfwSetCharCallback(mWindow, Callbacks::charCallbackWrapper);
  glfwSetDropCallback(mWindow, Callbacks::dropCallbackWrapper);
}

bool Window::isClosed() const {
  if (!mWindow) {
    return true;
  }
  return glfwWindowShouldClose(mWindow);
}

bool Window::isFullScreen() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowMonitor(mWindow) != NULL);
}

bool Window::isWindowed() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowMonitor(mWindow) == NULL);
}

bool Window::isIconified() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_ICONIFIED));
}

bool Window::isMaximized() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_MAXIMIZED));
}

bool Window::isVisible() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_VISIBLE));
}

bool Window::isFocused() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUSED));
}

bool Window::isTransparent() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_TRANSPARENT_FRAMEBUFFER));
}

bool Window::isDecorated() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_DECORATED));
}

bool Window::isResizable() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_RESIZABLE));
}

bool Window::isFloating() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_FLOATING));
}

bool Window::isAutoIconified() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_AUTO_ICONIFY));
}

bool Window::isFocusOnShowed() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_FOCUS_ON_SHOW));
}

bool Window::isPressing(Keyboard const &key) const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetKey(mWindow, (u32)key) == GLFW_PRESS);
}

bool Window::isEntering() const {
  if (!mWindow) {
    return false;
  }
  return bool(glfwGetWindowAttrib(mWindow, GLFW_HOVERED));
}

void Window::close() const {
  if (!mWindow) {
    return;
  }
  glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
}

void Window::iconify() const {
  if (!mWindow) {
    return;
  }
  glfwSetWindowAttrib(mWindow, GLFW_ICONIFIED, GLFW_TRUE);
}

void Window::maximize() const {
  if (!mWindow) {
    return;
  }
  glfwSetWindowAttrib(mWindow, GLFW_MAXIMIZED, GLFW_TRUE);
}

void Window::show() const {
  if (!mWindow) {
    return;
  }
  glfwSetWindowAttrib(mWindow, GLFW_VISIBLE, GLFW_TRUE);
}

void Window::hide() const {
  if (!mWindow) {
    return;
  }
  glfwSetWindowAttrib(mWindow, GLFW_VISIBLE, GLFW_FALSE);
}

void Window::focus() const {
  if (!mWindow) {
    return;
  }
  glfwSetWindowAttrib(mWindow, GLFW_FOCUSED, GLFW_TRUE);
}

void Window::restore() const {
  if (!mWindow) {
    return;
  }
  glfwRestoreWindow(mWindow);
}

void Window::requestAttention() const {
  if (!mWindow) {
    return;
  }
  glfwRequestWindowAttention(mWindow);
}

void Window::destroy() {
  if (mWindow) {
    glfwDestroyWindow(mWindow);
    mWindow = nullptr;
  }
}

void Window::swap() const {
  if (mWindow) {
    glfwSwapBuffers(mWindow);
  }
}

} // namespace Terreate::Graphic
