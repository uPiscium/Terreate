#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

#include "object.hpp"

namespace Terreate::OpenGL {
class ImageConverter;

struct TextureData {
  vec<ubyte> pixels;
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
  Object mTexture = Object();
  pair<u32> mSize = {0u, 0u};
  u32 mLayers = 0u;
  pair<FilterType> mFilter = {FilterType::LINEAR, FilterType::LINEAR};
  pair<WrappingType> mWrap = {WrappingType::REPEAT, WrappingType::REPEAT};
  umap<str, u32> mTextures = umap<str, u32>();

private:
  friend class Screen;
  friend class ImageConverter;

  Texture(Object const &texture, u32 const &width, u32 const &height,
          u32 const &layers)
      : mTexture(texture), mSize(width, height), mLayers(layers) {}

  void addBinding(str const &name) {
    mTextures.insert({name, mTextures.size()});
  }
  void addBinding(str const &name, u32 const &index) {
    mTextures.insert({name, index});
  }

public:
  Texture() { glGenTextures(1, mTexture); }
  Texture(u32 const &width, u32 const &height, u32 const &layers = 1);
  Texture(TextureSize const &size, u32 const &layers = 1);
  ~Texture();

  u32 const &getGLIndex() const { return mTexture; }
  u32 const &getTextureIndex(str const &name) const {
    return mTextures.at(name);
  }

  u32 const &getWidth() const { return mSize.first; }
  u32 const &getHeight() const { return mSize.second; }
  u32 const &getLayers() const { return mLayers; }
  u32 getCurrentLayer() const { return mTextures.size(); }

  void setFilter(FilterType const &min, FilterType const &mag);
  void setWrapping(WrappingType const &s, WrappingType const &t);

  void loadData(str const &name, u32 width, u32 height, u32 channels,
                ubyte const *data);
  void loadData(str const &name, TextureData const &data) {
    this->loadData(name, data.width, data.height, data.channels,
                   data.pixels.data());
  }
  void loadDataAt(str const &name, u32 const &xoffset, u32 const &yoffset,
                  u32 const &layer, u32 const &width, u32 const &height,
                  u32 const &channels, ubyte const *data);
  void loadDataAt(str const &name, u32 const &xoffset, u32 const &yoffset,
                  u32 const &layer, TextureData const &data) {
    this->loadDataAt(name, xoffset, yoffset, layer, data.width, data.height,
                     data.channels, data.pixels.data());
  }

  void bind() const { glBindTexture(GL_TEXTURE_2D_ARRAY, (u32)mTexture); }
  void unbind() const { glBindTexture(GL_TEXTURE_2D_ARRAY, 0); }

  u32 const &operator[](str const &name) const {
    return this->getTextureIndex(name);
  }

public:
  static u32 getMaxTextureSize();
  static u32 getMaxStorage();
  static u32 getMaxLayers();
};

class CubeTexture {
private:
  Object mTexture = Object();
  u32 mWidth = 0u;
  u32 mHeight = 0u;
  u32 mChannels = 0u;
  FilterType mFilter = FilterType::LINEAR;
  WrappingType mWrap = WrappingType::REPEAT;

private:
  friend class CubeScreen;

  CubeTexture(u32 const &texture, u32 const &width, u32 const &height,
              u32 const &channels)
      : mTexture(texture), mWidth(width), mHeight(height), mChannels(channels) {
  }

public:
  CubeTexture() { glGenTextures(1, mTexture); }
  ~CubeTexture();

  void setFilter(FilterType const &filter);
  void setWrapping(WrappingType const &wrap);

  void loadData(CubeFace face, u32 width, u32 height, u32 channels,
                ubyte const *data);
  void loadData(CubeFace face, TextureData const &data) {
    this->loadData(face, data.width, data.height, data.channels,
                   data.pixels.data());
  }
  void loadDatas(vec<TextureData> const &datas);

  void bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture); }
  void unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

  operator bool() const { return mTexture; }
};
} // namespace Terreate::OpenGL
