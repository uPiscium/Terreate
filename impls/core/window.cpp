#include <core/window.hpp>

namespace Terreate::Core {
void WindowSettings::apply() const {
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
  mImages = vec<GLFWimage>();
  mPointers = vec<u8 *>();
}

Icon::~Icon() {
  for (auto &pointer : mPointers) {
    delete[] pointer;
  }
}

u32 Icon::getImageCount() const { return mImages.size(); }

Icon::operator GLFWimage const *() const { return mImages.data(); }

Icon::operator bool() const { return mImages.size() > 0; }

void Icon::addImage(u32 const &width, u32 const &height, u8 const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  u8 *newPixels = new u8[width * height * 4];
  memcpy(newPixels, pixels, width * height * 4);
  image.pixels = newPixels;
  mPointers.push_back(newPixels);
  mImages.push_back(image);
}

Cursor::Cursor(i32 const &xHot, i32 const &yHot) : mXHot(xHot), mYHot(yHot) {
  mPixels = nullptr;
  mCursor = nullptr;
}

Cursor::~Cursor() {
  glfwDestroyCursor(mCursor);
  delete[] mPixels;
}

void Cursor::setImage(u32 const &width, u32 const &height, u8 const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  mPixels = new u8[width * height * 4];
  memcpy(mPixels, pixels, width * height * 4);
  image.pixels = mPixels;
  mCursor = glfwCreateCursor(&image, mXHot, mYHot);
}

Cursor::operator GLFWcursor *() const { return mCursor; }

Cursor::operator bool() const { return mCursor != nullptr; }

StandardCursor::StandardCursor(CursorShape const &shape) {
  if (shape == CursorShape::DEFAULT) {
    mCursor = nullptr;
  } else {
    mCursor = glfwCreateStandardCursor((u32)shape);
  }
}

StandardCursor::~StandardCursor() {
  if (mCursor != nullptr) {
    glfwDestroyCursor(mCursor);
  }
}

StandardCursor::operator GLFWcursor *() const { return mCursor; }

StandardCursor::operator bool() const { return mCursor != nullptr; }

Window::Window(Instance const &instance, str const &title,
               pair<i32> const &size, WindowSettings const &settings)
    : mInstance(instance) {
  settings.apply();
  mWindow = glfwCreateWindow(size.first, size.second, title.c_str(), nullptr,
                             nullptr);
  if (mWindow == nullptr) {
    throw Exception::WindowCreationFailure();
  }
  glfwSetWindowUserPointer(mWindow, this);

  if (glfwCreateWindowSurface(instance, mWindow, nullptr, &mSurface) !=
      VK_SUCCESS) {
    throw Exception::SurfaceCreationFailure();
  }

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
}

Window::~Window() { this->dispose(); }

void Window::dispose() {
  if (mWindow != nullptr) {
    glfwDestroyWindow(mWindow);
    mWindow = nullptr;
  }
  if (mSurface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    mSurface = VK_NULL_HANDLE;
  }
}

} // namespace Terreate::Core
