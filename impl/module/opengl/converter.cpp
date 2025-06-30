#include "../../../include/module/opengl/converter.hpp"
#include "../../../include/module/opengl/exception.hpp"
#include "../../../include/module/opengl/shader.hpp"

namespace Terreate::OpenGL {
str const ImageConverter::sKernelSource =
    "#version 430\n"
    "layout(local_size_x=" +
    std::to_string(ImageConverter::sKernelInputSize) +
    ", local_size_y=" + std::to_string(ImageConverter::sKernelInputSize) +
    ") in;" +
    R"(
uniform sampler2D inputTexture;
layout(rgba32f) uniform image2DArray outputTextures;
uniform vec2 textureSize;
uniform vec2 inputSize;
uniform vec2 outputSize;
uniform int layer;
void main() {
  ivec3 id = ivec3(gl_GlobalInvocationID);
  vec2 scale = inputSize / textureSize;
  vec2 inputUV = vec2(id.xy) / outputSize;
  vec4 inputColor = texture(inputTexture, inputUV * scale);
  imageStore(outputTextures, ivec3(id.xy, layer), inputColor);
}
    )";

void ImageConverter::createInputTexture() {
  u32 size = Texture::getMaxTextureSize() / 2;
  mKernel.setUniform("textureSize", vec2(size, size));
  glGenTextures(1, &mInputTexture);
  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, size, size);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

ImageConverter::ImageConverter() {
  /* mKernel.AddKernelSource(sKernelSource); */
  mKernel.addKernelSource(sKernelSource);
  mKernel.compile();
  mKernel.link();

  Shader::activateTexture(TextureTargets::TEX_0);
  mKernel.setUniform("inputTexture", 0);
  this->createInputTexture();
}

ImageConverter::~ImageConverter() {
  if (mInputTexture != 0) {
    glDeleteTextures(1, &mInputTexture);
    mInputTexture = 0;
  }
}

void ImageConverter::convert(str const &name, u32 const &width,
                             u32 const &height, u32 const &channels,
                             ubyte const *pixels, Texture &storage) {
  u32 dispatchX =
      (storage.getWidth() + (sKernelInputSize - 1)) / sKernelInputSize;
  u32 dispatchY =
      (storage.getHeight() + (sKernelInputSize - 1)) / sKernelInputSize;

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

  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format,
                  GL_UNSIGNED_BYTE, (void const *)pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glBindTexture(GL_TEXTURE_2D, 0);

  mKernel.bindImage("outputTextures", storage);

  Shader::activateTexture(TextureTargets::TEX_0);
  mKernel.setUniform("inputTexture", 0);
  mKernel.setUniform("inputSize", vec2(width, height));
  mKernel.setUniform("outputSize",
                     vec2(storage.getWidth(), storage.getHeight()));
  mKernel.setUniform("layer", (i32)storage.getCurrentLayer());
  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  mKernel.dispatch(dispatchX, dispatchY, 1);
  glBindTexture(GL_TEXTURE_2D, 0);
  storage.addBinding(name);
}

void ImageConverter::convert(str const &name, u32 const &index,
                             u32 const &width, u32 const &height,
                             u32 const &channels, ubyte const *pixels,
                             Texture &storage) {
  u32 dispatchX =
      (storage.getWidth() + (sKernelInputSize - 1)) / sKernelInputSize;
  u32 dispatchY =
      (storage.getHeight() + (sKernelInputSize - 1)) / sKernelInputSize;

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

  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format,
                  GL_UNSIGNED_BYTE, (void const *)pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glBindTexture(GL_TEXTURE_2D, 0);

  mKernel.bindImage("outputTextures", storage);

  Shader::activateTexture(TextureTargets::TEX_0);
  mKernel.setUniform("inputTexture", 0);
  mKernel.setUniform("inputSize", vec2(width, height));
  mKernel.setUniform("outputSize",
                     vec2(storage.getWidth(), storage.getHeight()));
  mKernel.setUniform("layer", (i32)index);
  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  mKernel.dispatch(dispatchX, dispatchY, 1);
  glBindTexture(GL_TEXTURE_2D, 0);
  storage.addBinding(name, index);
}
} // namespace Terreate::OpenGL
