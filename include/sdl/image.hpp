#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

namespace Terreate::SDL {
class RawImage {
private:
  PixelFormat mFormat = PixelFormat::UNKNOWN;
  pair<u32> mSize = {0, 0};
  u32 mPitch = 0;
  vec<byte> mPixelArray = {};

private:
  void allocate();

public:
  RawImage() = default;
  RawImage(PixelFormat const &format) : mFormat(format) {}
  RawImage(PixelFormat const &format, pair<u32> const &size) : mFormat(format) {
    this->allocate();
  }
  RawImage(PixelFormat const &format, pair<u32> const &size, u32 pitch,
           void *pixels);

  pair<u32> const &getSize() const { return mSize; }
  u32 const &getPitch() const { return mPitch; }
  void *getPixels() const { return (void *)mPixelArray.data(); }

  void setFormat(PixelFormat const &format);
  void setSize(pair<u32> const &size);
  void setPitch(u32 const &pitch) { mPitch = pitch; }

  void allocate(PixelFormat const &format, pair<u32> const &size);
  void loadData(void *pixels, pair<u32> const &size, PixelFormat const &format);
  void loadData(void *pixels, pair<u32> const &size) {
    this->loadData(pixels, size, mFormat);
  }
  void loadData(void *pixels) { this->loadData(pixels, mSize, mFormat); }
};
} // namespace Terreate::SDL
