#ifndef __TERREATE_IO_IMAGE_HPP__
#define __TERREATE_IO_IMAGE_HPP__

#include <core/type.hpp>

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
  u32 width;
  u32 height;
  u32 channels;
};

class Image {
public:
  static Texture load(Str const &filepath);
  static void save(Str const &filepath, Texture const &texture);
  static void savePNG(Str const &filepath, Texture const &texture);
  static void saveJPG(Str const &filepath, Texture const &texture,
                      i32 quality = 100);
  static void saveBMP(Str const &filepath, Texture const &texture);
  static void saveTGA(Str const &filepath, Texture const &texture);
  static void saveHDR(Str const &filepath, Texture const &texture);
};
} // namespace Terreate::IO

#endif // __TERREATE_IO_IMAGE_HPP__
