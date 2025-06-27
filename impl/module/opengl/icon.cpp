#include "../../../include/module/opengl/icon.hpp"

#include <cstring>

namespace Terreate::OpenGL {
Icon::Icon() {
  mImages = vec<GLFWimage>();
  mPointers = vec<ubyte *>();
}

Icon::~Icon() {
  for (auto &pointer : mPointers) {
    delete[] pointer;
  }
}

void Icon::addImage(u32 const &width, u32 const &height, ubyte const *pixels) {
  GLFWimage image;
  image.width = width;
  image.height = height;
  ubyte *newPixels = new ubyte[width * height * 4];
  std::memcpy(newPixels, pixels, width * height * 4);
  image.pixels = newPixels;
  mPointers.push_back(newPixels);
  mImages.push_back(image);
}
} // namespace Terreate::OpenGL
