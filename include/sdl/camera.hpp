#pragma once

#include "common/type.hpp"

#include "sdl/enum.hpp"

namespace Terreate::SDL {

struct CameraFrame {
public:
  u64 timestamp = 0;
  pair<u32> size = {0, 0};
  u32 pitch = 0;
  vec<u8> pixels; // Must be formatted RGBA32
};

class Camera {
private:
  CameraID mID;
  bool mOpened = false;
  SDL_Camera *mCamera = nullptr;

public:
  Camera(CameraID id);
  ~Camera();

  SDL_CameraID getID() const;
  CameraPermission getCurrentPermission() const;
  CameraPosition getPosition() const;
  str getName() const;
  CameraFrame getFrame() const;
  SDL_CameraSpec getSpec() const;
  vec<SDL_CameraSpec> getSupportedSpecs() const;
  PixelFormat getFormat() const;
  ColorSpace getColorSpace() const;
  pair<u32> getSize() const;
  float getSampleFrameRate() const;

  void setCameraSpec(PixelFormat format, ColorSpace colorSpace, pair<u32> size,
                     pair<u32> fpsRatio);

  bool const &isOpened() const;

  void open();
  void close();

public:
  static str getCurrentDriver();
  static vec<str> getDrivers();
  static vec<SDL_CameraID> getCameras();
};

} // namespace Terreate::SDL
