#include <cstring>
#include <exceptions.hpp>
#include <io/image.hpp>

namespace Terreate::IO {
Texture Image::Load(Str const &filepath) {
  Texture texture;
  Int width, height, channels;
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

void Image::Save(Str const &filepath, Texture const &texture) {
  Str extension = filepath.substr(filepath.find_last_of(".") + 1);
  if (extension == "png") {
    SavePNG(filepath, texture);
  } else if (extension == "jpg" || extension == "jpeg") {
    SaveJPG(filepath, texture);
  } else if (extension == "bmp") {
    SaveBMP(filepath, texture);
  } else if (extension == "tga") {
    SaveTGA(filepath, texture);
  } else if (extension == "hdr") {
    SaveHDR(filepath, texture);
  } else {
    throw Exceptions::FileException("Unsupported file format");
  }
}

void Image::SavePNG(Str const &filepath, Texture const &texture) {
  stbi_write_png(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data(),
                 texture.width * texture.channels);
}

void Image::SaveJPG(Str const &filepath, Texture const &texture, Int quality) {
  stbi_write_jpg(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data(), quality);
}

void Image::SaveBMP(Str const &filepath, Texture const &texture) {
  stbi_write_bmp(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data());
}

void Image::SaveTGA(Str const &filepath, Texture const &texture) {
  stbi_write_tga(filepath.c_str(), texture.width, texture.height,
                 texture.channels, texture.data.data());
}

void Image::SaveHDR(Str const &filepath, Texture const &texture) {
  stbi_write_hdr(filepath.c_str(), texture.width, texture.height,
                 texture.channels, (Float *)texture.data.data());
}

} // namespace Terreate::IO
