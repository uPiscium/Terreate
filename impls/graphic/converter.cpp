#include <exceptions.hpp>
#include <graphic/converter.hpp>
#include <graphic/shader.hpp>

namespace Terreate::Graphic::Compute {
Str const ImageConverter::sKernelSource =
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

void ImageConverter::CreateInputTexture() {
  Uint size = Texture::GetMaxTextureSize() / 2;
  mKernel.SetVec2("textureSize", {size, size});
  glGenTextures(1, mInputTexture);
  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, size, size);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

ImageConverter::ImageConverter() {
  /* mKernel.AddKernelSource(sKernelSource); */
  mKernel.AddKernelSource(sKernelSource);
  mKernel.Compile();
  mKernel.Link();

  Shader::ActivateTexture(TextureTargets::TEX_0);
  mKernel.SetInt("inputTexture", 0);
  this->CreateInputTexture();
}

ImageConverter::~ImageConverter() {
  if (mInputTexture.Count() <= 1) {
    glDeleteTextures(1, mInputTexture);
    mInputTexture.Delete();
  }
}

void ImageConverter::Convert(Str const &name, Uint const &width,
                             Uint const &height, Uint const &channels,
                             Ubyte const *pixels, Texture &storage) {
  Uint dispatchX =
      (storage.GetWidth() + (sKernelInputSize - 1)) / sKernelInputSize;
  Uint dispatchY =
      (storage.GetHeight() + (sKernelInputSize - 1)) / sKernelInputSize;

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

  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format,
                  GL_UNSIGNED_BYTE, (void const *)pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glBindTexture(GL_TEXTURE_2D, 0);

  mKernel.BindImage("outputTextures", storage);

  Shader::ActivateTexture(TextureTargets::TEX_0);
  mKernel.SetInt("inputTexture", 0);
  mKernel.SetVec2("inputSize", vec2(width, height));
  mKernel.SetVec2("outputSize", vec2(storage.GetWidth(), storage.GetHeight()));
  mKernel.SetInt("layer", storage.GetCurrentLayer());
  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  mKernel.Dispatch(dispatchX, dispatchY, 1);
  glBindTexture(GL_TEXTURE_2D, 0);
  storage.AddBinding(name);
}

void ImageConverter::Convert(Str const &name, Uint const &index,
                             Uint const &width, Uint const &height,
                             Uint const &channels, Ubyte const *pixels,
                             Texture &storage) {
  Uint dispatchX =
      (storage.GetWidth() + (sKernelInputSize - 1)) / sKernelInputSize;
  Uint dispatchY =
      (storage.GetHeight() + (sKernelInputSize - 1)) / sKernelInputSize;

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

  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format,
                  GL_UNSIGNED_BYTE, (void const *)pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glBindTexture(GL_TEXTURE_2D, 0);

  mKernel.BindImage("outputTextures", storage);

  Shader::ActivateTexture(TextureTargets::TEX_0);
  mKernel.SetInt("inputTexture", 0);
  mKernel.SetVec2("inputSize", vec2(width, height));
  mKernel.SetVec2("outputSize", vec2(storage.GetWidth(), storage.GetHeight()));
  mKernel.SetInt("layer", index);
  glBindTexture(GL_TEXTURE_2D, mInputTexture);
  mKernel.Dispatch(dispatchX, dispatchY, 1);
  glBindTexture(GL_TEXTURE_2D, 0);
  storage.AddBinding(name, index);
}
} // namespace Terreate::Graphic::Compute
