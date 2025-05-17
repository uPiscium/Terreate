#ifndef __TERREATE_GRAPHICS_CONVERTER_HPP__
#define __TERREATE_GRAPHICS_CONVERTER_HPP__

#include <graphic/GLdefs.hpp>
#include <graphic/compute.hpp>
#include <graphic/texture.hpp>

namespace Terreate::Graphic::Compute {
using namespace Terreate::Types;

class ImageConverter final {
private:
  static u32 const sKernelInputSize = 16;
  static Str const sKernelSource;

private:
  Core::Object mInputTexture;
  ComputeKernel mKernel;

private:
  void CreateInputTexture();

public:
  /*
   * @brief Construct a new Image Converter object
   */
  ImageConverter();
  ~ImageConverter();

  /*
   * @brief: Stretch the image data to a texture
   * @param: name: The name of the texture
   * @param: width: The width of the image
   * @param: height: The height of the image
   * @param: channels: The number of channels in the image
   * @param: pixels: The pixel data of the image
   * @param: storage: The texture to store the data
   */
  void convert(Str const &name, u32 const &width, u32 const &height,
               u32 const &channels, Ubyte const *pixels, Texture &storage);
  /*
   * @brief: Stretch the image data to specified layer of texture
   * @param: name: The name of the texture
   * @param: index: The layer of the texture
   * @param: width: The width of the image
   * @param: height: The height of the image
   * @param: channels: The number of channels in the image
   * @param: pixels: The pixel data of the image
   * @param: storage: The texture to store the data
   */
  void convert(Str const &name, u32 const &index, u32 const &width,
               u32 const &height, u32 const &channels, Ubyte const *pixels,
               Texture &storage);
};
} // namespace Terreate::Graphic::Compute

#endif // __TERREATE_GRAPHICS_CONVERTER_HPP__
