#ifndef __TERREATE_IO_IMAGE_HPP__
#define __TERREATE_IO_IMAGE_HPP__

#include <types.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#else
#undef STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#else
#undef STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#define STB_IMAGE_WRITE_STATIC
#include <stb/stb_image_write.h>

namespace Terreate::IO {
using namespace Terreate::Types;

struct Texture {
  Vec<Ubyte> data;
  Uint width;
  Uint height;
  Uint channels;
};

class Image {
public:
  static Texture Load(Str const &filepath);
  static void Save(Str const &filepath, Texture const &texture);
  static void SavePNG(Str const &filepath, Texture const &texture);
  static void SaveJPG(Str const &filepath, Texture const &texture,
                      Int quality = 100);
  static void SaveBMP(Str const &filepath, Texture const &texture);
  static void SaveTGA(Str const &filepath, Texture const &texture);
  static void SaveHDR(Str const &filepath, Texture const &texture);
};
} // namespace Terreate::IO

#endif // __TERREATE_IO_IMAGE_HPP__
