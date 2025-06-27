#include "../../../include/module/common/exception.hpp"
#include "../../../include/module/opengl/screen.hpp"

namespace Terreate::OpenGL {
Screen::Screen(u32 const &width, u32 const &height, u32 const &layers)
    : mWidth(width), mHeight(height), mLayers(layers) {
  glGenFramebuffers(1, &mFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
  u32 rbo = 0;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  GLObject buffer = 0;
  glGenTextures(1, &buffer);
  glBindTexture(GL_TEXTURE_2D_ARRAY, buffer);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, mWidth, mHeight, layers);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  for (u32 i = 0; i < layers; i++) {
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, buffer,
                              0, i);
    mDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  Texture *texture = new Texture(buffer, mWidth, mHeight, layers);
  mTexture = unique<Texture>(texture);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Screen::~Screen() {
  if (mFrameBuffer != 0) {
    glDeleteFramebuffers(1, &mFrameBuffer);
    mFrameBuffer = 0;
  }
}

void Screen::transcript(Screen const &screen) const {
  this->bind(BindMode::READ_ONLY);
  screen.bind(BindMode::WRITE_ONLY);
  glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, screen.mWidth, screen.mHeight,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
  this->unbind();
}

void Screen::bind(BindMode mode) const {
  switch (mode) {
  case BindMode::READ_ONLY:
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);
    break;
  case BindMode::WRITE_ONLY:
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
    break;
  case BindMode::READ_WRITE:
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    break;
  default:
    throw Exception::ScreenError("Invalid bind mode for Screen.");
  }
  glViewport(0, 0, mWidth, mHeight);
  glDrawBuffers(mDrawBuffers.size(), mDrawBuffers.data());
}

void Screen::unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK);
}

void Screen::fill(vec<float> const &color) {
  this->bind();
  glClearColor(color[0], color[1], color[2], 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  this->unbind();
}

void Screen::clear() {
  this->bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  this->unbind();
}
} // namespace Terreate::OpenGL
