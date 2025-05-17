#pragma once
#include <core/IDisposable.hpp>
#include <core/api.hpp>
#include <core/instance.hpp>
#include <core/winevent.hpp>
#include <type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

class WindowSettings {
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

public:
  WindowSettings() = default;
  void apply() const;
};

class Icon final {
private:
  vec<GLFWimage> mImages;
  vec<u8 *> mPointers;

private:
  Icon(Icon const &) = delete;
  Icon &operator=(Icon const &) = delete;

public:
  /*
   * @brief: This function creates a glfw icon.
   */
  Icon();
  ~Icon();

  /*
   * @brief: This function returns the number of images in the icon.
   * @return: The number of images in the icon.
   */
  u32 getImageCount() const;

  /*
   * @brief: This function adds an image to the icon.
   * @param: width: Image width.
   * @param: height: Image height.
   * @param: pixels: Image pixels.
   * @detail: Pixels are copied to new allocated array and set to "glfw
   image"
   * instance. "glfw image" is used to render icon. Icon can have multiple of
   * images.
   */
  void addImage(u32 const &width, u32 const &height, u8 const *pixels);

  operator GLFWimage const *() const;
  operator bool() const;
};

class Cursor final {
private:
  GLFWcursor *mCursor = nullptr;
  GLFWimage mImage = GLFWimage();
  u8 *mPixels = nullptr;
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
  Cursor(i32 const &xHot = 0, i32 const &yHot = 0);
  ~Cursor();

  /*
   * @brief: This function sets the image of the cursor.
   * @param: width: Image width.
   * @param: height: Image height.
   * @param: pixels: Image pixels.
   * @detail: The image is copied to new allocated array and set to "glfw
   * image" instance. "glfw image" is used to render cursor.
   */
  void setImage(u32 const &width, u32 const &height, u8 const *pixels);

  operator GLFWcursor *() const;
  operator bool() const;
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
  StandardCursor(CursorShape const &shape);
  ~StandardCursor();

  operator GLFWcursor *() const;
  operator bool() const;
};

class Window : public IDisposable {
private:
  GLFWwindow *mWindow = nullptr;
  Instance mInstance;
  VkSurfaceKHR mSurface = VK_NULL_HANDLE;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;

public:
  WindowProperties properties;
  WindowEvent events;

public:
  Window(Instance const &instance, str const &title, pair<i32> const &size,
         WindowSettings const &settings);
  ~Window();

  void dispose() override;
};
} // namespace Terreate::Core
