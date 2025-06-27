#pragma once

#include "../common/type.hpp"

namespace Terreate::OpenGL {
class Icon {
private:
  vec<GLFWimage> mImages;
  vec<ubyte *> mPointers;

private:
  Icon(Icon const &) = delete;
  Icon &operator=(Icon const &) = delete;

public:
  Icon();
  ~Icon();

  u32 getSize() const { return mImages.size(); }

  void addImage(u32 const &width, u32 const &height, ubyte const *pixels);

  operator GLFWimage const *() const { return mImages.data(); }
  operator bool() const { return mImages.size() > 0; }
};
} // namespace Terreate::OpenGL
