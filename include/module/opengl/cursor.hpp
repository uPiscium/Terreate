#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

namespace Terreate::OpenGL {
class Cursor {
private:
  PROHIBIT_COPY_AND_ASSIGN(Cursor);

private:
  GLFWcursor *mCursor = nullptr;
  GLFWimage mImage = GLFWimage();
  ubyte *mPixels = nullptr;
  i32 mXHot = 0;
  i32 mYHot = 0;

public:
  Cursor(i32 const &xHot = 0, i32 const &yHot = 0) : mXHot(xHot), mYHot(yHot) {}
  ~Cursor();

  void setImage(u32 const &width, u32 const &height, ubyte const *pixels);

  operator GLFWcursor *() const { return mCursor; }
  operator bool() const { return mCursor != nullptr; }
};

class StandardCursor {
private:
  PROHIBIT_COPY_AND_ASSIGN(StandardCursor);

private:
  GLFWcursor *mCursor = nullptr;

public:
  StandardCursor(CursorShape const &shape) {
    mCursor = shape != CursorShape::DEFAULT
                  ? glfwCreateStandardCursor((u32)shape)
                  : nullptr;
  }
  ~StandardCursor() { glfwDestroyCursor(mCursor); }

  operator GLFWcursor *() const { return mCursor; }
  operator bool() const { return mCursor != nullptr; }
};
} // namespace Terreate::OpenGL
