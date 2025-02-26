#include <exceptions.hpp>
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

void Icon::AddImage(Uint const &width, Uint const &height,
                    Ubyte const *pixels) {
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

void Cursor::SetImage(Uint const &width, Uint const &height,
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
void WindowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->position = Pair<Int>(xpos, ypos);
  ptr->onPositionChange.Publish(ptr, xpos, ypos);
}

void WindowSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->size = Pair<Uint>(width, height);
  ptr->onSizeChange.Publish(ptr, width, height);
}

void WindowCloseCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onClose.Publish(ptr);
}

void WindowRefreshCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onRefresh.Publish(ptr);
}

void WindowFocusCallbackWrapper(GLFWwindow *window, int focused) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onFocus.Publish(ptr, (Bool)focused);
}

void WindowIconifyCallbackWrapper(GLFWwindow *window, int iconified) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onIconify.Publish(ptr, (Bool)iconified);
}

void WindowMaximizeCallbackWrapper(GLFWwindow *window, int maximized) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onMaximize.Publish(ptr, (Bool)maximized);
}

void WindowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                          int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onFramebufferSizeChange.Publish(ptr, width, height);
}

void WindowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onContentScaleChange.Publish(ptr, xscale, yscale);
}

void MousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
                                int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onMousebuttonInput.Publish(ptr, button, action, Modifier(mods));
}

void CursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->cursorPosition = Pair<Double>(xpos, ypos);
  ptr->onCursorPositionChange.Publish(ptr, xpos, ypos);
}

void CursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->onCursorEnter.Publish(ptr, (Bool)entered);
}

void ScrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->scrollOffset = Pair<Double>(xoffset, yoffset);
  ptr->onScroll.Publish(ptr, xoffset, yoffset);
}

void KeyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Key wrappedKey = Key(key, scancode, action, mods);
  ptr->keys->push_back(wrappedKey);
  ptr->onKeyInput.Publish(ptr, wrappedKey);
}

void CharCallbackWrapper(GLFWwindow *window, Uint codepoint) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->codePoints->push_back(codepoint);
  ptr->onCharInput.Publish(ptr, codepoint);
}

void DropCallbackWrapper(GLFWwindow *window, int count, const char **paths) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Vec<Str> droppedFiles(paths, paths + count);
  ptr->droppedFiles = droppedFiles;
  ptr->onFileDrop.Publish(ptr, droppedFiles);
}
} // namespace Callbacks

Window::Window(Uint const &width, Uint const &height, Str const &title,
               WindowSettings const &settings) {
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

  mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  glfwSetWindowUserPointer(mWindow, this);
  glfwMakeContextCurrent(mWindow);

  this->scrollOffset.Set(&mScrollOffset);
  this->codePoints.Set(&mCodePoints);
  this->keys.Set(&mKeys);
  this->droppedFiles.Set(&mDroppedFiles);

  this->size.Set(&mSize);
  this->position.Set(&mPosition);
  this->cursorPosition.Set(&mCursorPosition);
  this->clipboard.Set(&mClipboard);
  this->title.Set(&mTitle);
  this->opacity.Set(&mOpacity);

  this->size.getter = [this]() {
    glfwGetWindowSize(mWindow, &mSize.first, &mSize.second);
    return mSize;
  };
  this->size.setter = [this](Pair<Uint> const &size) {
    glfwSetWindowSize(mWindow, size.first, size.second);
    mSize = size;
  };
  this->position.getter = [this]() {
    glfwGetWindowPos(mWindow, &mPosition.first, &mPosition.second);
    return mPosition;
  };
  this->position.setter = [this](Pair<Int> const &position) {
    glfwSetWindowPos(mWindow, position.first, position.second);
    mPosition = position;
  };
  this->cursorPosition.getter = [this]() {
    glfwGetCursorPos(mWindow, &mCursorPosition.first, &mCursorPosition.second);
    return mCursorPosition;
  };
  this->cursorPosition.setter = [this](Pair<Double> const &position) {
    glfwSetCursorPos(mWindow, position.first, position.second);
    mCursorPosition = position;
  };
  this->clipboard.getter = [this]() {
    mClipboard = glfwGetClipboardString(NULL);
    return mClipboard;
  };
  this->clipboard.setter = [this](Str const &text) {
    glfwSetClipboardString(mWindow, text.c_str());
    mClipboard = text;
  };
  this->title.getter = [this]() {
    mTitle = glfwGetWindowTitle(mWindow);
    return mTitle;
  };
  this->title.setter = [this](Str const &title) {
    glfwSetWindowTitle(mWindow, title.c_str());
    mTitle = title;
  };
  this->opacity.getter = [this]() {
    mOpacity = glfwGetWindowOpacity(mWindow);
    return mOpacity;
  };
  this->opacity.setter = [this](Float const &opacity) {
    glfwSetWindowOpacity(mWindow, opacity);
    mOpacity = opacity;
  };

  glfwSetWindowPosCallback(mWindow, Callbacks::WindowPositionCallbackWrapper);
  glfwSetWindowSizeCallback(mWindow, Callbacks::WindowSizeCallbackWrapper);
  glfwSetWindowCloseCallback(mWindow, Callbacks::WindowCloseCallbackWrapper);
  glfwSetWindowRefreshCallback(mWindow,
                               Callbacks::WindowRefreshCallbackWrapper);
  glfwSetWindowFocusCallback(mWindow, Callbacks::WindowFocusCallbackWrapper);
  glfwSetWindowIconifyCallback(mWindow,
                               Callbacks::WindowIconifyCallbackWrapper);
  glfwSetWindowMaximizeCallback(mWindow,
                                Callbacks::WindowMaximizeCallbackWrapper);
  glfwSetFramebufferSizeCallback(
      mWindow, Callbacks::WindowFramebufferSizeCallbackWrapper);
  glfwSetWindowContentScaleCallback(
      mWindow, Callbacks::WindowContentScaleCallbackWrapper);
  glfwSetMouseButtonCallback(mWindow, Callbacks::MousebuttonCallbackWrapper);
  glfwSetCursorPosCallback(mWindow, Callbacks::CursorPositionCallbackWrapper);
  glfwSetCursorEnterCallback(mWindow, Callbacks::CursorEnterCallbackWrapper);
  glfwSetScrollCallback(mWindow, Callbacks::ScrollCallbackWrapper);
  glfwSetKeyCallback(mWindow, Callbacks::KeyCallbackWrapper);
  glfwSetCharCallback(mWindow, Callbacks::CharCallbackWrapper);
  glfwSetDropCallback(mWindow, Callbacks::DropCallbackWrapper);
}

void Window::Destroy() {
  glfwDestroyWindow(mWindow);
  mWindow = nullptr;
}
} // namespace Terreate::Graphic
