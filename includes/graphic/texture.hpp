#ifndef __TERREATE_GRAPHICS_TEXTURE_HPP__
#define __TERREATE_GRAPHICS_TEXTURE_HPP__

#include <bindable.hpp>
#include <core/object.hpp>
#include <graphic/GLdefs.hpp>
#include <types.hpp>

namespace Terreate::Graphic::Compute {
class ImageConverter;
} // namespace Terreate::Graphic::Compute

namespace Terreate::Graphic {
using namespace Terreate::Types;

struct TextureData {
  Vec<Ubyte> pixels;
  Uint width = 0u;
  Uint height = 0u;
  Uint channels = 0u;
};

enum class TextureSize {
  S256 = 256,
  S512 = 512,
  S1024 = 1024,
  S2048 = 2048,
  S4096 = 4096,
  S8192 = 8192,
  S16384 = 16384,
  S32768 = 32768,
};

class Texture : public Interface::IBindable {
private:
  Core::Object mTexture = Core::Object();
  Pair<Uint> mSize = {0u, 0u};
  Uint mLayers = 0u;
  Pair<FilterType> mFilter = {FilterType::LINEAR, FilterType::LINEAR};
  Pair<WrappingType> mWrap = {WrappingType::REPEAT, WrappingType::REPEAT};
  Map<Str, Uint> mTextures = Map<Str, Uint>();

private:
  friend class Screen;
  friend class Compute::ImageConverter;
  /*
   * @brief: DO NOT USE THIS CONSTRUCTOR.
   * This constructor should only be called by Screen class.
   * @param: texture: OpenGL texture ID
   * @param: width: width of texture
   * @param: height: height of texture
   */
  Texture(Core::Object const &texture, Uint const &width, Uint const &height,
          Uint const &layers)
      : mTexture(texture), mSize(width, height), mLayers(layers) {}

  void AddBinding(Str const &name) {
    mTextures.insert({name, mTextures.size()});
  }
  void AddBinding(Str const &name, Uint const &index) {
    mTextures.insert({name, index});
  }

public:
  /*
   * @brief: This function creates a opengl texture set.
   */
  Texture() { glGenTextures(1, mTexture); }
  /*
   * @brief: This function creates a opengl texture set.
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: layers: number of layers in texture
   * @param: type: type of texture channel
   */
  Texture(Uint const &width, Uint const &height, Uint const &layers = 1);
  /*
   * @brief: This function creates a opengl texture set.
   * @param: size: size of texture (width and height are the same and its a
   * power of 2)
   * @param: layers: number of layers in texture
   */
  Texture(TextureSize const &size, Uint const &layers = 1);
  ~Texture();

  /*
   * @brief: Getter for OpenGL texture ID.
   * @return: OpenGL texture ID
   */
  Uint const &GetGLIndex() const { return mTexture; }
  /*
   * @brief: Getter for texture index.
   * @param: name: name of texture
   * @return: texture index
   */
  Uint const &GetTextureIndex(Str const &name) const {
    return mTextures.at(name);
  }

  /*
   * @brief: Getter for texture width.
   * @return: texture width
   */
  Uint const &GetWidth() const { return mSize.first; }
  /*
   * @brief: Getter for texture height.
   * @return: texture height
   */
  Uint const &GetHeight() const { return mSize.second; }
  /*
   * @brief: Getter for texture size.
   * @return: texture size
   */
  Uint const &GetLayers() const { return mLayers; }
  /*
   * @brief: Getter for current empty layer.
   * @return: current empty layer
   */
  Uint GetCurrentLayer() const { return mTextures.size(); }

  /*
   * @brief: Setter for texture filter.
   * @param: min: min filter type
   * @param: mag: mag filter type
   */
  void SetFilter(FilterType const &min, FilterType const &mag);
  /*
   * @brief: Setter for texture filter.
   * @param: filter: filter type
   */
  void SetWrapping(WrappingType const &s, WrappingType const &t);

  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: name: name of texture
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   * @param: data: pointer to texture data
   */
  void LoadData(Str const &name, Uint width, Uint height, Uint channels,
                Ubyte const *data);
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: name: name of texture
   * @param: data: texture data
   */
  void LoadData(Str const &name, TextureData const &data) {
    this->LoadData(name, data.width, data.height, data.channels,
                   data.pixels.data());
  }
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: name: name of texture
   * @param: xoffset: x offset of texture
   * @param: yoffset: y offset of texture
   * @param: layer: layer of texture
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   * @param: data: pointer to texture data
   */
  void LoadDataAt(Str const &name, Uint const &xoffset, Uint const &yoffset,
                  Uint const &layer, Uint const &width, Uint const &height,
                  Uint const &channels, Ubyte const *data);
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: name: name of texture
   * @param: xoffset: x offset of texture
   * @param: yoffset: y offset of texture
   * @param: layer: layer of texture
   * @param: data: texture data
   */
  void LoadDataAt(Str const &name, Uint const &xoffset, Uint const &yoffset,
                  Uint const &layer, TextureData const &data) {
    this->LoadDataAt(name, xoffset, yoffset, layer, data.width, data.height,
                     data.channels, data.pixels.data());
  }

  /*
   * @brief: Binds texture to OpenGL.
   */
  void Bind() const override {
    glBindTexture(GL_TEXTURE_2D_ARRAY, (TCu32)mTexture);
  }
  /*
   * @brief: Unbinds texture from OpenGL.
   */
  void Unbind() const override { glBindTexture(GL_TEXTURE_2D_ARRAY, 0); }

  Uint const &operator[](Str const &name) const {
    return this->GetTextureIndex(name);
  }

public:
  /*
   * @brief: Getter for maximum texture size.
   * @return: maximum texture size
   */
  static Uint GetMaxTextureSize();
  /*
   * @brief: Getter for maximum texture storage.
   * @return: maximum texture storage
   */
  static Uint GetMaxStorage();
  /*
   * @brief: Getter for maximum texture layers.
   * @return: maximum texture layers
   */
  static Uint GetMaxLayers();
};

class CubeTexture : public Interface::IBindable {
private:
  Core::Object mTexture = Core::Object();
  Uint mWidth = 0u;
  Uint mHeight = 0u;
  Uint mChannels = 0u;
  FilterType mFilter = FilterType::LINEAR;
  WrappingType mWrap = WrappingType::REPEAT;

public:
  /*
   * @brief: This function creates a opengl cube texture.
   */
  CubeTexture() { glGenTextures(1, mTexture); }
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
  ~CubeTexture();

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
  void Bind() const override { glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture); }
  /*
   * @brief: Unbinds texture from OpenGL.
   */
  void Unbind() const override { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

  operator Bool() const { return mTexture; }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_TEXTURE_HPP__
