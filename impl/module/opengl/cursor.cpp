#include "../../../include/module/opengl/cursor.hpp"

#include <cstring>

namespace Terreate::OpenGL {
Cursor::~Cursor() {
  glfwDestroyCursor(mCursor);
  delete[] mPixels;
}

void Cursor::setImage(u32 const &width, u32 const &height,
                      ubyte const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  mPixels = new ubyte[width * height * 4];
  std::memcpy(mPixels, pixels, width * height * 4);
  image.pixels = mPixels;
  mCursor = glfwCreateCursor(&image, mXHot, mYHot);
}
} // namespace Terreate::OpenGL
