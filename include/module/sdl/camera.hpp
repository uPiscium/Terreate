#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

#include "image.hpp"

namespace Terreate::SDL {
struct CameraFrame {
public:
  u64 timestamp = 0;
  SDLImage image;
};

class Camera {
private:
  SDL_CameraID mID;
  bool mOpened = false;
  SDL_Camera *mCamera = nullptr;

public:
  Camera(SDL_CameraID id) : mID(id) {}
  ~Camera() { this->close(); }

  SDL_CameraID getID() const { return mID; }
  CameraPermission getCurrentPermission() const {
    return (CameraPermission)SDL_GetCameraPermissionState(mCamera);
  }
  CameraPosition getPosition() const {
    return (CameraPosition)SDL_GetCameraPosition(mID);
  }
  str getName() const { return SDL_GetCameraName(mID); }
  CameraFrame getFrame() const;
  SDL_CameraSpec getSpec() const;
  vec<SDL_CameraSpec> getSupportedSpecs() const;
  PixelFormat getFormat() const;
  ColorSpace getColorSpace() const;
  pair<u32> getSize() const;
  float getSampleFrameRate() const;

  void setCameraSpec(PixelFormat format, ColorSpace colorSpace, pair<u32> size,
                     pair<u32> fpsRatio);

  bool const &isOpened() const { return mOpened; }

  void open();
  void close();

public:
  static str getCurrentDriver() { return SDL_GetCurrentCameraDriver(); }
  static vec<str> getDrivers();
  static vec<SDL_CameraID> getCameras();
};
} // namespace Terreate::SDL
