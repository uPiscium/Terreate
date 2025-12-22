#include "core/sdl/camera.hpp"
#include "core/sdl/exception.hpp"

namespace Terreate::Core::SDL {

Camera::Camera(CameraID id) : mID(id), mCamera(nullptr), mOpened(false) {}

Camera::~Camera() { this->close(); }

CameraID Camera::getID() const { return mID; }

CameraPermission Camera::getCurrentPermission() const {
  return (CameraPermission)SDL_GetCameraPermissionState(mCamera);
}

CameraPosition Camera::getPosition() const {
  return (CameraPosition)SDL_GetCameraPosition(mID);
}

str Camera::getName() const { return SDL_GetCameraName(mID); }

CameraFrame Camera::getFrame() const {
  CameraFrame frame;
  SDL_Surface *surf = SDL_AcquireCameraFrame(mCamera, &frame.timestamp);
  if (!surf) {
    throw CameraException("Failed to acquire camera frame for ID: " +
                          std::to_string(mID));
  }
  SDL_Surface *converted =
      SDL_ConvertSurface(surf, (SDL_PixelFormat)PixelFormat::RGBA32);
  if (!converted) {
    throw CameraException("Failed to acquire camera frame for ID: " +
                          std::to_string(mID));
  }
  frame.size = {converted->w, converted->h};
  frame.pitch = converted->pitch;
  frame.pixels.resize(converted->h * converted->pitch);
  std::memcpy(frame.pixels.data(), converted->pixels, frame.pixels.size());
  SDL_ReleaseCameraFrame(mCamera, surf);
  SDL_DestroySurface(converted);
  return frame;
}

SDL_CameraSpec Camera::getSpec() const {
  SDL_CameraSpec spec;
  if (!SDL_GetCameraFormat(mCamera, &spec)) {
    throw CameraException("Failed to get camera spec for ID: " +
                          std::to_string(mID));
  }
  return spec;
}

vec<SDL_CameraSpec> Camera::getSupportedSpecs() const {
  i32 count = 0;
  SDL_CameraSpec **ptr = SDL_GetCameraSupportedFormats(mID, &count);
  if (!ptr) {
    throw CameraException("Failed to get supported camera specs for ID: " +
                          std::to_string(mID));
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
  SDL_CameraSpec spec = this->getSpec();
  return (PixelFormat)spec.format;
}

ColorSpace Camera::getColorSpace() const {
  SDL_CameraSpec spec = this->getSpec();
  return (ColorSpace)spec.colorspace;
}

pair<u32> Camera::getSize() const {
  SDL_CameraSpec spec = this->getSpec();
  return {spec.width, spec.height};
}

float Camera::getSampleFrameRate() const {
  SDL_CameraSpec spec = this->getSpec();
  return static_cast<float>(spec.framerate_numerator) /
         spec.framerate_denominator;
}

bool const &Camera::isOpened() const { return mOpened; }

void Camera::open() {
  mCamera = SDL_OpenCamera(mID, nullptr);
  if (!mCamera) {
    throw CameraException("Failed to get camera with ID: " +
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

str Camera::getCurrentDriver() { return SDL_GetCurrentCameraDriver(); }

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

} // namespace Terreate::Core::SDL
