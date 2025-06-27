#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

#include "object.hpp"
#include "texture.hpp"

namespace Terreate::OpenGL {
class Screen {
private:
  Object mFrameBuffer = Object();
  u32 mWidth;
  u32 mHeight;
  u32 mLayers;
  i32 mInitialViewPort[4];
  Texture mTexture;
  vec<GLenum> mDrawBuffers;

public:
  Screen(u32 const &width, u32 const &height, u32 const &layers = 1);
  ~Screen();

  u32 getWidth() const { return mWidth; }
  u32 getHeight() const { return mHeight; }
  Texture const &getTexture() const { return mTexture; }

  void transcript(Screen const &screen) const;
  void readOnlyBind() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);
  }
  void drawOnlyBind() const;
  void bind();
  void unbind() const;
  void fill(vec<float> const &color);
  void clear();

  operator bool() const { return mFrameBuffer; }
};
} // namespace Terreate::OpenGL
