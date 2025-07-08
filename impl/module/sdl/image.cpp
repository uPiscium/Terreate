#include "../../../include/module/sdl/exception.hpp"
#include "../../../include/module/sdl/image.hpp"

#include <cstring>

namespace Terreate::SDL {
void RawImage::allocate() {
  u8 pixelSize = SDL_BITSPERPIXEL((SDL_PixelFormat)mFormat);
  mPixelArray.resize(mSize.first * mSize.second * pixelSize);
}

RawImage::RawImage(PixelFormat const &format, pair<u32> const &size, u32 pitch,
                   void *pixels)
    : mFormat(format), mSize(size), mPitch(pitch) {
  this->allocate();
  this->loadData(pixels, size, format);
}

void RawImage::setFormat(PixelFormat const &format) {
  if (mFormat != format) {
    mFormat = format;
    this->allocate();
  }
}

void RawImage::setSize(pair<u32> const &size) {
  if (mSize != size) {
    mSize = size;
    this->allocate();
  }
}

void RawImage::allocate(PixelFormat const &format, pair<u32> const &size) {
  if (mFormat != format || mSize != size) {
    mFormat = format;
    mSize = size;
    this->allocate();
  }
}

void RawImage::loadData(void *pixels, pair<u32> const &size,
                        PixelFormat const &format) {
  if (mFormat != format || mSize != size) {
    this->allocate(format, size);
  }
  if (pixels) {
    std::memcpy(mPixelArray.data(), pixels, mPixelArray.size());
  } else {
    std::memset(mPixelArray.data(), 0, mPixelArray.size());
  }
}
} // namespace Terreate::SDL
