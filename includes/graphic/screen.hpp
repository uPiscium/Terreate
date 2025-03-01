#ifndef __TERREATE_GRAPHICS_SCREEN_HPP__
#define __TERREATE_GRAPHICS_SCREEN_HPP__

#include <core/object.hpp>
#include <graphic/GLdefs.hpp>
#include <graphic/texture.hpp>
#include <types.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

class Screen {
private:
  Core::Object mFrameBuffer = Core::Object();
  Uint mWidth;
  Uint mHeight;
  Uint mLayers;
  Int mInitialViewPort[4];
  Texture mTexture;
  Vec<GLenum> mDrawBuffers;

public:
  /*
   * @brief: Constructor for Screen.
   * @param: width: width of screen
   * @param: height: height of screen
   * @param: layers: number of screen layers
   */
  Screen(Uint const &width, Uint const &height, Uint const &layers = 1);
  ~Screen();

  /*
   * @brief: Getter for width.
   * @return: width
   */
  Uint GetWidth() const { return mWidth; }
  /*
   * @brief: Getter for height.
   * @return: height
   */
  Uint GetHeight() const { return mHeight; }
  /*
   * @brief: Getter for texture set.
   * @return: texture set
   */
  Texture const &GetTexture() const { return mTexture; }

  /*
   * @brief: Transcript to other screen.
   * @param: screen: screen to transcript to
   */
  void Transcript(Screen const &screen) const;
  /*
   * @brief: Binds the screen for reading.
   */
  void ReadOnlyBind() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);
  }
  /*
   * @brief: Binds the screen for drawing.
   */
  void DrawOnlyBind() const;
  /*
   * @brief: Binds the screen.
   */
  void Bind();
  /*
   * @brief: Unbinds the screen.
   */
  void Unbind() const;
  /*
   * @brief: Fill screen with color.
   * @param: color: color to fill screen with
   * @detail: Color format is (red, green, blue). Each color is float (0
   * ~ 1.0).
   */
  void Fill(Vec<Float> const &color);
  /*
   * @brief: Clear screen.
   */
  void Clear();

  operator Bool() const { return mFrameBuffer; }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_SCREEN_HPP__
