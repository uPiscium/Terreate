#include <opengl/exception.hpp>
#include <opengl/texture.hpp>

namespace Terreate::OpenGL {
Texture::Texture(u32 const &width, u32 const &height, u32 const &layers)
    : mSize({width, height}), mLayers(layers) {
  glGenTextures(1, &mTexture);

  this->bind();
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, mSize.first, mSize.second,
                 layers);
  this->setFilter(mFilter.first, mFilter.second);
  this->setWrapping(mWrap.first, mWrap.second);
  this->unbind();
}

Texture::Texture(TextureSize const &size, u32 const &layers)
    : mSize({(u32)size, (u32)size}), mLayers(layers) {
  glGenTextures(1, &mTexture);

  this->bind();
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, mSize.first, mSize.second,
                 layers);
  this->setFilter(mFilter.first, mFilter.second);
  this->setWrapping(mWrap.first, mWrap.second);
  this->unbind();
}

Texture::~Texture() {
  if (mTexture != 0) {
    glDeleteTextures(1, &mTexture);
    mTexture = 0;
  }
}

void Texture::setFilter(FilterType const &min, FilterType const &mag) {
  mFilter = {min, mag};
  this->bind();
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, (GLenum)min);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, (GLenum)mag);
  this->unbind();
}

void Texture::setWrapping(WrappingType const &s, WrappingType const &t) {
  mWrap = {s, t};
  this->bind();
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, (GLenum)s);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, (GLenum)t);
  this->unbind();
}

void Texture::loadData(str const &name, u32 width, u32 height, u32 channels,
                       ubyte const *data) {
  mTextures[name] = mTextures.size();

  u32 format;
  switch (channels) {
  case 1:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    format = GL_RED;
    break;
  case 2:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    format = GL_RG;
    break;
  case 3:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  default:
    throw Exception::TextureError("Invalid number of channels.");
  }

  this->bind();
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, mSize.second - height,
                  mTextures[name], width, height, 1, format, GL_UNSIGNED_BYTE,
                  data);
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  this->unbind();
}

void Texture::loadDataAt(str const &name, u32 const &xoffset,
                         u32 const &yoffset, u32 const &layer, u32 const &width,
                         u32 const &height, u32 const &channels,
                         ubyte const *data) {
  mTextures[name] = layer;

  u32 format;
  switch (channels) {
  case 1:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    format = GL_RED;
    break;
  case 2:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    format = GL_RG;
    break;
  case 3:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  default:
    throw Exception::TextureError("Invalid number of channels.");
  }

  this->bind();
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, xoffset, yoffset, layer, width,
                  height, 1, format, GL_UNSIGNED_BYTE, data);
  // glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  this->unbind();
}

u32 Texture::getMaxTextureSize() {
  GLint size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
  return size;
}

u32 Texture::getMaxStorage() {
  u32 size = Texture::getMaxTextureSize();
  return size * size;
}

u32 Texture::getMaxLayers() {
  GLint layers;
  glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layers);
  return layers;
}

CubeTexture::~CubeTexture() {
  if (mTexture != 0) {
    glDeleteTextures(1, &mTexture);
    mTexture = 0;
  }
}

void CubeTexture::setFilter(FilterType const &filter) {
  mFilter = filter;
  this->bind();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLenum)mFilter);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLenum)mFilter);
  this->unbind();
}

void CubeTexture::setWrapping(WrappingType const &wrap) {
  mWrap = wrap;
  this->bind();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLenum)mWrap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLenum)mWrap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (GLenum)mWrap);
  this->unbind();
}

void CubeTexture::loadData(CubeFace face, u32 width, u32 height, u32 channels,
                           ubyte const *data) {
  mWidth = width;
  mHeight = height;
  mChannels = channels;
  GLenum format;
  switch (mChannels) {
  case 1:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    format = GL_RED;
    break;
  case 2:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    format = GL_RG;
    break;
  case 3:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  default:
    format = 0;
    break;
  }

  if (format == 0) {
    throw Exception::TextureError("Invalid number of channels.");
    return;
  }

  this->bind();
  glTexImage2D((GLenum)face, 0, GL_RGBA, mWidth, mHeight, 0, format,
               GL_UNSIGNED_BYTE, data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  this->setFilter(mFilter);
  this->setWrapping(mWrap);
  this->unbind();
}

void CubeTexture::loadDatas(vec<TextureData> const &datas) {
  for (u32 i = 0; i < datas.size(); ++i) {
    this->loadData((CubeFace)((GLenum)CubeFace::RIGHT + i), datas[i]);
  }
}
} // namespace Terreate::OpenGL
