#ifndef __TERREATE_GRAPHICS_TEXTURE_HPP__
#define __TERREATE_GRAPHICS_TEXTURE_HPP__

#include <core/object.hpp>
#include <core/type.hpp>
#include <graphic/GLdefs.hpp>

namespace Terreate::Graphic::Compute {
class ImageConverter;
} // namespace Terreate::Graphic::Compute

namespace Terreate::Graphic {
using namespace Terreate::Types;

struct TextureData {
  Vec<Ubyte> pixels;
  u32 width = 0u;
  u32 height = 0u;
  u32 channels = 0u;
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

class Texture {
private:
  Core::Object mTexture = Core::Object();
  Pair<u32> mSize = {0u, 0u};
  u32 mLayers = 0u;
  Pair<FilterType> mFilter = {FilterType::LINEAR, FilterType::LINEAR};
  Pair<WrappingType> mWrap = {WrappingType::REPEAT, WrappingType::REPEAT};
  Map<Str, u32> mTextures = Map<Str, u32>();

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
  Texture(Core::Object const &texture, u32 const &width, u32 const &height,
          u32 const &layers)
      : mTexture(texture), mSize(width, height), mLayers(layers) {}

  void addBinding(Str const &name) {
    mTextures.insert({name, mTextures.size()});
  }
  void addBinding(Str const &name, u32 const &index) {
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
  Texture(u32 const &width, u32 const &height, u32 const &layers = 1);
  /*
   * @brief: This function creates a opengl texture set.
   * @param: size: size of texture (width and height are the same and its a
   * power of 2)
   * @param: layers: number of layers in texture
   */
  Texture(TextureSize const &size, u32 const &layers = 1);
  ~Texture();

  /*
   * @brief: Getter for OpenGL texture ID.
   * @return: OpenGL texture ID
   */
  u32 const &getGLIndex() const { return mTexture; }
  /*
   * @brief: Getter for texture index.
   * @param: name: name of texture
   * @return: texture index
   */
  u32 const &getTextureIndex(Str const &name) const {
    return mTextures.at(name);
  }

  /*
   * @brief: Getter for texture width.
   * @return: texture width
   */
  u32 const &getWidth() const { return mSize.first; }
  /*
   * @brief: Getter for texture height.
   * @return: texture height
   */
  u32 const &getHeight() const { return mSize.second; }
  /*
   * @brief: Getter for texture size.
   * @return: texture size
   */
  u32 const &getLayers() const { return mLayers; }
  /*
   * @brief: Getter for current empty layer.
   * @return: current empty layer
   */
  u32 getCurrentLayer() const { return mTextures.size(); }

  /*
   * @brief: Setter for texture filter.
   * @param: min: min filter type
   * @param: mag: mag filter type
   */
  void setFilter(FilterType const &min, FilterType const &mag);
  /*
   * @brief: Setter for texture filter.
   * @param: filter: filter type
   */
  void setWrapping(WrappingType const &s, WrappingType const &t);

  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: name: name of texture
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   * @param: data: pointer to texture data
   */
  void loadData(Str const &name, u32 width, u32 height, u32 channels,
                Ubyte const *data);
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: name: name of texture
   * @param: data: texture data
   */
  void loadData(Str const &name, TextureData const &data) {
    this->loadData(name, data.width, data.height, data.channels,
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
  void loadDataAt(Str const &name, u32 const &xoffset, u32 const &yoffset,
                  u32 const &layer, u32 const &width, u32 const &height,
                  u32 const &channels, Ubyte const *data);
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: name: name of texture
   * @param: xoffset: x offset of texture
   * @param: yoffset: y offset of texture
   * @param: layer: layer of texture
   * @param: data: texture data
   */
  void loadDataAt(Str const &name, u32 const &xoffset, u32 const &yoffset,
                  u32 const &layer, TextureData const &data) {
    this->loadDataAt(name, xoffset, yoffset, layer, data.width, data.height,
                     data.channels, data.pixels.data());
  }

  /*
   * @brief: Binds texture to OpenGL.
   */
  void bind() const { glBindTexture(GL_TEXTURE_2D_ARRAY, (u32)mTexture); }
  /*
   * @brief: Unbinds texture from OpenGL.
   */
  void unbind() const { glBindTexture(GL_TEXTURE_2D_ARRAY, 0); }

  u32 const &operator[](Str const &name) const {
    return this->getTextureIndex(name);
  }

public:
  /*
   * @brief: Getter for maximum texture size.
   * @return: maximum texture size
   */
  static u32 getMaxTextureSize();
  /*
   * @brief: Getter for maximum texture storage.
   * @return: maximum texture storage
   */
  static u32 getMaxStorage();
  /*
   * @brief: Getter for maximum texture layers.
   * @return: maximum texture layers
   */
  static u32 getMaxLayers();
};

class CubeTexture {
private:
  Core::Object mTexture = Core::Object();
  u32 mWidth = 0u;
  u32 mHeight = 0u;
  u32 mChannels = 0u;
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
  CubeTexture(u32 const &texture, u32 const &width, u32 const &height,
              u32 const &channels)
      : mTexture(texture), mWidth(width), mHeight(height), mChannels(channels) {
  }
  ~CubeTexture();

  /*
   * @brief: Setter for texture filter.
   * @param: filter: filter type
   */
  void setFilter(FilterType const &filter);
  /*
   * @brief: Setter for texture wrapping.
   * @param: wrap: wrapping type
   */
  void setWrapping(WrappingType const &wrap);

  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: face: face of cube texture
   * @param: width: width of texture
   * @param: height: height of texture
   * @param: channels: number of channels in texture
   * @param: data: pointer to texture data
   */
  void loadData(CubeFace face, u32 width, u32 height, u32 channels,
                Ubyte const *data);
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: face: face of cube texture
   * @param: data: texture data
   */
  void loadData(CubeFace face, TextureData const &data) {
    this->loadData(face, data.width, data.height, data.channels,
                   data.pixels.data());
  }
  /*
   * @brief: Loads texture data into OpenGL texture.
   * @param: datas: texture data
   * @detail: datas should be in the order of
   * {right, left, top, bottom, front, back}
   */
  void loadDatas(Vec<TextureData> const &datas);

  /*
   * @brief: Binds texture to OpenGL.
   */
  void bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture); }
  /*
   * @brief: Unbinds texture from OpenGL.
   */
  void unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

  operator bool() const { return mTexture; }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_TEXTURE_HPP__
