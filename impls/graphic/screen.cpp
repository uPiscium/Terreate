#include <graphic/screen.hpp>

namespace Terreate::Graphic {
Screen::Screen(Uint const &width, Uint const &height, Uint const &layers)
    : mWidth(width), mHeight(height), mLayers(layers) {
  glGenFramebuffers(1, mFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
  ID rbo = 0;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  Core::Object buffer = Core::Object();
  glGenTextures(1, buffer);
  glBindTexture(GL_TEXTURE_2D_ARRAY, buffer);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, mWidth, mHeight, layers);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  for (Uint i = 0; i < layers; i++) {
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, buffer,
                              0, i);
    mDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  mTexture = Texture(buffer, mWidth, mHeight, layers);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Screen::~Screen() {
  if (mFrameBuffer.Count() <= 1) {
    glDeleteFramebuffers(1, mFrameBuffer);
    mFrameBuffer.Delete();
  }
}

void Screen::Transcript(Screen const &screen) const {
  this->ReadOnlyBind();
  screen.DrawOnlyBind();
  glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, screen.mWidth, screen.mHeight,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
  this->Unbind();
}

void Screen::DrawOnlyBind() const {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
  glDrawBuffers(mDrawBuffers.size(), mDrawBuffers.data());
}

void Screen::Bind() {
  glGetIntegerv(GL_VIEWPORT, mInitialViewPort);
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
  glViewport(0, 0, mWidth, mHeight);
  glDrawBuffers(mDrawBuffers.size(), mDrawBuffers.data());
}

void Screen::Unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(mInitialViewPort[0], mInitialViewPort[1], mInitialViewPort[2],
             mInitialViewPort[3]);
  glDrawBuffer(GL_BACK);
}

void Screen::Fill(Vec<Float> const &color) {
  this->Bind();
  glClearColor(color[0], color[1], color[2], 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  this->Unbind();
}

void Screen::Clear() {
  this->Bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  this->Unbind();
}
} // namespace Terreate::Graphic
