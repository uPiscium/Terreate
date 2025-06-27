#pragma once

#include "compute.hpp"
#include "texture.hpp"

// TODO: Move converter module to more high level module

namespace Terreate::OpenGL {
class ImageConverter {
private:
  static u32 const sKernelInputSize = 16;
  static str const sKernelSource;

private:
  GLObject mInputTexture;
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
