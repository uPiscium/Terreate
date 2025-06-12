#include "../../include/vulkan/window.hpp"

namespace Terreate::Vulkan {
void WindowSettings::apply() const {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUSED, focused ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_AUTO_ICONIFY, autoIconify ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_FLOATING, floating ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_MAXIMIZED, maximized ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_CENTER_CURSOR, centerCursor ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                 transparentFramebuffer ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, focusOnShow ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR,
                 scaleToMonitor ? GLFW_TRUE : GLFW_FALSE);
}

Icon::Icon() {
  mImages = Type::vec<GLFWimage>();
  mPointers = Type::vec<Type::u8 *>();
}

Icon::~Icon() {
  for (auto &pointer : mPointers) {
    delete[] pointer;
  }
}

Type::u32 Icon::getImageCount() const { return mImages.size(); }

Icon::operator GLFWimage const *() const { return mImages.data(); }

Icon::operator bool() const { return mImages.size() > 0; }

void Icon::addImage(Type::u32 const &width, Type::u32 const &height,
                    Type::u8 const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  Type::u8 *newPixels = new Type::u8[width * height * 4];
  memcpy(newPixels, pixels, width * height * 4);
  image.pixels = newPixels;
  mPointers.push_back(newPixels);
  mImages.push_back(image);
}

Cursor::Cursor(Type::i32 const &xHot, Type::i32 const &yHot)
    : mXHot(xHot), mYHot(yHot) {
  mPixels = nullptr;
  mCursor = nullptr;
}

Cursor::~Cursor() {
  glfwDestroyCursor(mCursor);
  delete[] mPixels;
}

void Cursor::setImage(Type::u32 const &width, Type::u32 const &height,
                      Type::u8 const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  mPixels = new Type::u8[width * height * 4];
  memcpy(mPixels, pixels, width * height * 4);
  image.pixels = mPixels;
  mCursor = glfwCreateCursor(&image, mXHot, mYHot);
}

Cursor::operator GLFWcursor *() const { return mCursor; }

Cursor::operator bool() const { return mCursor != nullptr; }

StandardCursor::StandardCursor(Type::CursorShape const &shape) {
  if (shape == Type::CursorShape::DEFAULT) {
    mCursor = nullptr;
  } else {
    mCursor = glfwCreateStandardCursor((Type::u32)shape);
  }
}

StandardCursor::~StandardCursor() {
  if (mCursor != nullptr) {
    glfwDestroyCursor(mCursor);
  }
}

StandardCursor::operator GLFWcursor *() const { return mCursor; }

StandardCursor::operator bool() const { return mCursor != nullptr; }

void Window::destroy() {
  if (mWindow != nullptr) {
    glfwDestroyWindow(mWindow);
    mWindow = nullptr;
  }
}

Window::Window(VkInstance instance, Type::str const &title,
               Type::pair<Type::i32> const &size,
               WindowSettings const &settings)
    : mInstance(instance) {
  settings.apply();
  mWindow = glfwCreateWindow(size.first, size.second, title.c_str(), nullptr,
                             nullptr);
  if (mWindow == nullptr) {
    char const *log = nullptr;
    glfwGetError(&log);
    throw Exception::WindowCreationFailure(log);
  }
  glfwSetWindowUserPointer(mWindow, this);

  glfwSetWindowSizeCallback(mWindow, Wrapper::windowSizeCallbackWrapper);
  glfwSetWindowPosCallback(mWindow, Wrapper::windowPositionCallbackWrapper);
  glfwSetWindowCloseCallback(mWindow, Wrapper::windowCloseCallbackWrapper);
  glfwSetWindowRefreshCallback(mWindow, Wrapper::windowRefreshCallbackWrapper);
  glfwSetWindowFocusCallback(mWindow, Wrapper::windowFocusCallbackWrapper);
  glfwSetWindowIconifyCallback(mWindow, Wrapper::windowIconifyCallbackWrapper);
  glfwSetWindowMaximizeCallback(mWindow,
                                Wrapper::windowMaximizeCallbackWrapper);
  glfwSetWindowContentScaleCallback(mWindow,
                                    Wrapper::windowContentScaleCallbackWrapper);
  glfwSetFramebufferSizeCallback(mWindow,
                                 Wrapper::windowFramebufferSizeCallbackWrapper);
  glfwSetMouseButtonCallback(mWindow, Wrapper::mousebuttonCallbackWrapper);
  glfwSetCursorPosCallback(mWindow, Wrapper::cursorPositionCallbackWrapper);
  glfwSetCursorEnterCallback(mWindow, Wrapper::cursorEnterCallbackWrapper);
  glfwSetScrollCallback(mWindow, Wrapper::scrollCallbackWrapper);
  glfwSetKeyCallback(mWindow, Wrapper::keyCallbackWrapper);
  glfwSetCharCallback(mWindow, Wrapper::charCallbackWrapper);
  glfwSetDropCallback(mWindow, Wrapper::dropCallbackWrapper);

  mProperties.setup(mWindow);
}

void Window::setIcon(Icon const &icon) {
  if (icon) {
    glfwSetWindowIcon(mWindow, icon.getImageCount(), icon);
  } else {
    glfwSetWindowIcon(mWindow, 0, nullptr);
  }
}

void Window::setCursor(Cursor const &cursor) {
  if (cursor) {
    glfwSetCursor(mWindow, cursor);
  } else {
    glfwSetCursor(mWindow, nullptr);
  }
}

void Window::setCursor(StandardCursor const &cursor) {
  if (cursor) {
    glfwSetCursor(mWindow, cursor);
  } else {
    glfwSetCursor(mWindow, nullptr);
  }
}

void Window::close() {
  if (mWindow != nullptr) {
    glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
    this->destroy();
  }
}
} // namespace Terreate::Vulkan
