#ifndef __TC_TEXTURE_HPP__
#define __TC_TEXTURE_HPP__

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "defines.hpp"
#include "object.hpp"

namespace TerreateCore {
namespace Core {
using namespace TerreateCore::Defines;

struct TextureData {
  Vec<Ubyte> pixels;
  Uint width = 0;
  Uint height = 0;
  Uint channels = 0;
};

class Texture final : public Object {
private:
  ID mTexture = 0;
  Uint mWidth = 0;
  Uint mHeight = 0;
  Uint mChannels = 0;
  FilterType mFilter = FilterType::LINEAR;
  WrappingType mWrap = WrappingType::REPEAT;

private:
  TC_DISABLE_COPY_AND_ASSIGN(Texture);

public:
  /*
   * @brief: This function creates a opengl texture.
   */
  Texture() { glGenTextures(1, &mTexture); }
  /*
   * @brief: DO NOT USE THIS CONSTRUCTOR.
   * This constructor should only be created by Screen.
   * @param: texture: OpenGL texture ID
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   */
  Texture(Uint const &texture, Uint const &width, Uint const &height,
          Uint const &channels)
      : mTexture(texture), mWidth(width), mHeight(height), mChannels(channels) {
  }
  ~Texture() override { glDeleteTextures(1, &mTexture); }

  /*
   * @brief: Setter for texture filter.
   * @param: filter: filter type
   */
  void SetFilter(FilterType const &filter);
  /*
   * @brief: Setter for texture wrapping.
   * @param: wrap: wrapping type
   */
  void SetWrapping(WrappingType const &wrap);

  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   * @param: data: pointer to texture data
   */
  void LoadData(Uint width, Uint height, Uint channels, Ubyte const *data);
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: data: texture data
   */
  void LoadData(TextureData const &data) {
    this->LoadData(data.width, data.height, data.channels, data.pixels.data());
  }

  /*
   * @brief: Binds texture to OpenGL.
   */
  void Bind() const { glBindTexture(GL_TEXTURE_2D, mTexture); }
  /*
   * @brief: Unbinds texture from OpenGL.
   */
  void Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

  operator bool() const override { return mTexture != 0; }

public:
  /*
   * @brief: Loads texture data from file.
   * @param: path: path to texture file
   * @return: texture data
   */
  static TextureData LoadTexture(Str const &path);
};

class CubeTexture final : public Object {
private:
  Uint mTexture = 0;
  Uint mWidth = 0;
  Uint mHeight = 0;
  Uint mChannels = 0;
  FilterType mFilter = FilterType::LINEAR;
  WrappingType mWrap = WrappingType::REPEAT;

private:
  TC_DISABLE_COPY_AND_ASSIGN(CubeTexture);

private:
  /*
   * @brief: DO NOT USE THIS CONSTRUCTOR.
   * This constructor should only be created by CubeScreen.
   * @param: texture: OpenGL texture ID
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   */
  CubeTexture(Uint const &texture, Uint const &width, Uint const &height,
              Uint const &channels)
      : mTexture(texture), mWidth(width), mHeight(height), mChannels(channels) {
  }

public:
  /*
   * @brief: This function creates a opengl cube texture.
   */
  CubeTexture() { glGenTextures(1, &mTexture); }
  ~CubeTexture() override { glDeleteTextures(1, &mTexture); }

  /*
   * @brief: Setter for texture filter.
   * @param: filter: filter type
   */
  void SetFilter(FilterType const &filter);
  /*
   * @brief: Setter for texture wrapping.
   * @param: wrap: wrapping type
   */
  void SetWrapping(WrappingType const &wrap);

  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: face: face of cube texture
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   * @param: data: pointer to texture data
   */
  void LoadData(CubeFace face, Uint width, Uint height, Uint channels,
                Ubyte const *data);
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: face: face of cube texture
   * @param: data: texture data
   */
  void LoadData(CubeFace face, TextureData const &data) {
    this->LoadData(face, data.width, data.height, data.channels,
                   data.pixels.data());
  }
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: datas: texture data
   * @detail: datas should be in the order of
   * {right, left, top, bottom, front, back}
   */
  void LoadDatas(Vec<TextureData> const &datas);

  /*
   * @brief: Binds texture to OpenGL.
   */
  void Bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture); }
  /*
   * @brief: Unbinds texture from OpenGL.
   */
  void Unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

  operator Bool() const override { return mTexture != 0; }
};
} // namespace Core
} // namespace TerreateCore

#endif // __TC_TEXTURE_HPP__
