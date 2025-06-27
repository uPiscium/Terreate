#pragma once

#include "../common/enum.hpp"

#include "compute.hpp"
#include "object.hpp"
#include "texture.hpp"

namespace Terreate::OpenGL {
class ImageConverter {
private:
  static u32 const sKernelInputSize = 16;
  static str const sKernelSource;

private:
  Object mInputTexture;
  ComputeKernel mKernel;

private:
  void createInputTexture();

public:
  ImageConverter();
  ~ImageConverter();

  void convert(str const &name, u32 const &width, u32 const &height,
               u32 const &channels, ubyte const *pixels, Texture &storage);
  void convert(str const &name, u32 const &index, u32 const &width,
               u32 const &height, u32 const &channels, ubyte const *pixels,
               Texture &storage);
};
} // namespace Terreate::OpenGL
