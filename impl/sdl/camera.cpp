#include <sdl/camera.hpp>
#include <sdl/exception.hpp>

namespace Terreate::SDL {
CameraFrame Camera::getFrame() const {
  CameraFrame frame;
  SDL_Surface *surf = SDL_AcquireCameraFrame(mCamera, &frame.timestamp);
  if (!surf) {
    throw Exception::SDLModuleError("Failed to acquire camera frame for ID: " +
                                    std::to_string(mID));
  }
  frame.image.loadData(surf->pixels, {surf->w, surf->h},
                       (PixelFormat)surf->format);
  frame.image.setPitch(surf->pitch);
  SDL_ReleaseCameraFrame(mCamera, surf);
  return frame;
}

SDL_CameraSpec Camera::getSpec() const {
  SDL_CameraSpec spec;
  if (!SDL_GetCameraFormat(mCamera, &spec)) {
    throw Exception::SDLModuleError("Failed to get camera spec for ID: " +
                                    std::to_string(mID));
  }
  return spec;
}

vec<SDL_CameraSpec> Camera::getSupportedSpecs() const {
  i32 count = 0;
  SDL_CameraSpec **ptr = SDL_GetCameraSupportedFormats(mID, &count);
  if (!ptr) {
    throw Exception::SDLModuleError(
        "Failed to get supported camera specs for ID: " + std::to_string(mID));
  }
  if (count == 0) {
    return {};
  }
  vec<SDL_CameraSpec> specs;
  specs.reserve(count);
  for (i32 i = 0; i < count; ++i) {
    specs.push_back(*ptr[i]);
  }
  SDL_free(ptr);
  return specs;
}

PixelFormat Camera::getFormat() const {
  SDL_CameraSpec spec = getSpec();
  return (PixelFormat)spec.format;
}

ColorSpace Camera::getColorSpace() const {
  SDL_CameraSpec spec = getSpec();
  return (ColorSpace)spec.colorspace;
}

pair<u32> Camera::getSize() const {
  SDL_CameraSpec spec = getSpec();
  return {spec.width, spec.height};
}

float Camera::getSampleFrameRate() const {
  SDL_CameraSpec spec = getSpec();
  return static_cast<float>(spec.framerate_numerator) /
         spec.framerate_denominator;
}

void Camera::open() {
  mCamera = SDL_OpenCamera(mID, nullptr);
  if (!mCamera) {
    throw Exception::SDLModuleError("Failed to get camera with ID: " +
                                    std::to_string(mID));
  }
  mOpened = true;
}

void Camera::close() {
  if (mOpened && mCamera) {
    SDL_CloseCamera(mCamera);
    mCamera = nullptr;
    mOpened = false;
  }
}

vec<str> Camera::getDrivers() {
  i32 numCameras = SDL_GetNumCameraDrivers();
  vec<str> drivers;
  for (int i = 0; i < numCameras; ++i) {
    drivers.push_back(SDL_GetCameraDriver(i));
  }
  return drivers;
}

vec<SDL_CameraID> Camera::getCameras() {
  i32 count = 0;
  SDL_CameraID *ptr = SDL_GetCameras(&count);
  vec<SDL_CameraID> cameras(ptr, ptr + count);
  return cameras;
}
} // namespace Terreate::SDL
