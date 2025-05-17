#include <core/exception.hpp>
#include <io/image.hpp>

#include <cstring>

namespace Terreate::IO {
Texture Image::load(Str const &filepath) {
  Texture texture;
  i32 width, height, channels;
  Ubyte const *data =
      stbi_load(filepath.c_str(), &width, &height, &channels, 0);
  if (!data) {
    throw Exceptions::FileException("Failed to load image");
  }
  texture.data.resize(width * height * channels);
  std::memcpy(texture.data.data(), data, width * height * channels);
  stbi_image_free((void *)data);
  texture.width = width;
  texture.height = height;
  texture.channels = channels;
  return texture;
}

void Image::save(Str const &filepath, Texture const &texture) {
  Str extension = filepath.substr(filepath.find_last_of(".") + 1);
  if (extension == "png") {
    Image::savePNG(filepath, texture);
  } else if (extension == "jpg" || extension == "jpeg") {
    Image::saveJPG(filepath, texture);
  } else if (extension == "bmp") {
    Image::saveBMP(filepath, texture);
  } else if (extension == "tga") {
    Image::saveTGA(filepath, texture);
  } else if (extension == "hdr") {
    Image::saveHDR(filepath, texture);
  } else {
    throw Exceptions::FileException("Unsupported file format");
  }
}

void Image::savePNG(Str const &filepath, Texture const &texture) {
  stbi_write_png(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data(),
                 texture.width * texture.channels);
}

void Image::saveJPG(Str const &filepath, Texture const &texture, i32 quality) {
  stbi_write_jpg(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data(), quality);
}

void Image::saveBMP(Str const &filepath, Texture const &texture) {
  stbi_write_bmp(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data());
}

void Image::saveTGA(Str const &filepath, Texture const &texture) {
  stbi_write_tga(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data());
}

void Image::saveHDR(Str const &filepath, Texture const &texture) {
  stbi_write_hdr(filepath.c_str(), texture.width, texture.height,
                 texture.channels, (float *)texture.data.data());
}

} // namespace Terreate::IO
