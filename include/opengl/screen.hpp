#pragma once

#include "../common/type.hpp"

#include "texture.hpp"

namespace Terreate::OpenGL {
enum class BindMode { READ_ONLY, WRITE_ONLY, READ_WRITE };

class Screen {
private:
  GLObject mFrameBuffer = 0;
  u32 mWidth;
  u32 mHeight;
  u32 mLayers;
  unique<Texture> mTexture;
  vec<GLenum> mDrawBuffers;

public:
  Screen(u32 const &width, u32 const &height, u32 const &layers = 1);
  ~Screen();

  u32 getWidth() const { return mWidth; }
  u32 getHeight() const { return mHeight; }
  Texture *getTexture() const { return mTexture.get(); }

  void transcript(Screen const &screen) const;
  void bind(BindMode mode = BindMode::READ_WRITE) const;
  void unbind() const;
  void fill(vec<float> const &color);
  void clear();

  operator bool() const { return mFrameBuffer; }
};
} // namespace Terreate::OpenGL
