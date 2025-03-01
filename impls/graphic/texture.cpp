#include <exceptions.hpp>
#include <graphic/texture.hpp>

namespace Terreate::Graphic {
Texture::Texture(Uint const &width, Uint const &height, Uint const &layers)
    : mSize({width, height}), mLayers(layers) {
  glGenTextures(1, mTexture);

  this->Bind();
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, mSize.first, mSize.second,
                 layers);
  this->SetFilter(mFilter.first, mFilter.second);
  this->SetWrapping(mWrap.first, mWrap.second);
  this->Unbind();
}

Texture::Texture(TextureSize const &size, Uint const &layers)
    : mSize({(Uint)size, (Uint)size}), mLayers(layers) {
  glGenTextures(1, mTexture);

  this->Bind();
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, mSize.first, mSize.second,
                 layers);
  this->SetFilter(mFilter.first, mFilter.second);
  this->SetWrapping(mWrap.first, mWrap.second);
  this->Unbind();
}

Texture::~Texture() {
  if (mTexture.Count() <= 1) {
    glDeleteTextures(1, mTexture);
    mTexture.Delete();
  }
}

void Texture::SetFilter(FilterType const &min, FilterType const &mag) {
  mFilter = {min, mag};
  this->Bind();
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, (GLenum)min);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, (GLenum)mag);
  this->Unbind();
}

void Texture::SetWrapping(WrappingType const &s, WrappingType const &t) {
  mWrap = {s, t};
  this->Bind();
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, (GLenum)s);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, (GLenum)t);
  this->Unbind();
}

void Texture::LoadData(Str const &name, Uint width, Uint height, Uint channels,
                       Ubyte const *data) {
  mTextures[name] = mTextures.size();

  Uint format;
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
    throw Exceptions::TextureError("Invalid number of channels.");
  }

  this->Bind();
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, mSize.second - height,
                  mTextures[name], width, height, 1, format, GL_UNSIGNED_BYTE,
                  data);
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  this->Unbind();
}

void Texture::LoadDataAt(Str const &name, Uint const &xoffset,
                         Uint const &yoffset, Uint const &layer,
                         Uint const &width, Uint const &height,
                         Uint const &channels, Ubyte const *data) {
  mTextures[name] = layer;

  Uint format;
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
    throw Exceptions::TextureError("Invalid number of channels.");
  }

  this->Bind();
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, xoffset, yoffset, layer, width,
                  height, 1, format, GL_UNSIGNED_BYTE, data);
  // glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  this->Unbind();
}

Uint Texture::GetMaxTextureSize() {
  GLint size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
  return size;
}

Uint Texture::GetMaxStorage() {
  GLuint size = Texture::GetMaxTextureSize();
  return size * size;
}

Uint Texture::GetMaxLayers() {
  GLint layers;
  glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layers);
  return layers;
}

CubeTexture::~CubeTexture() {
  if (mTexture.Count() <= 1) {
    glDeleteTextures(1, mTexture);
    mTexture.Delete();
  }
}

void CubeTexture::SetFilter(FilterType const &filter) {
  mFilter = filter;
  this->Bind();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLenum)mFilter);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLenum)mFilter);
  this->Unbind();
}

void CubeTexture::SetWrapping(WrappingType const &wrap) {
  mWrap = wrap;
  this->Bind();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLenum)mWrap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLenum)mWrap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (GLenum)mWrap);
  this->Unbind();
}

void CubeTexture::LoadData(CubeFace face, Uint width, Uint height,
                           Uint channels, Ubyte const *data) {
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
    throw Exceptions::TextureError("Invalid number of channels.");
    return;
  }

  this->Bind();
  glTexImage2D((GLenum)face, 0, GL_RGBA, mWidth, mHeight, 0, format,
               GL_UNSIGNED_BYTE, data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  this->SetFilter(mFilter);
  this->SetWrapping(mWrap);
  this->Unbind();
}

void CubeTexture::LoadDatas(Vec<TextureData> const &datas) {
  for (Uint i = 0; i < datas.size(); ++i) {
    this->LoadData((CubeFace)((GLenum)CubeFace::RIGHT + i), datas[i]);
  }
}
} // namespace Terreate::Graphic
