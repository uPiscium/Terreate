#include <sdl/exception.hpp>
#include <sdl/registry.hpp>

namespace Terreate::SDL {
shared<AudioDevice> ObjectRegistry::getAudioDevice(SDL_AudioDeviceID id) const {
  auto it = mAudioDevices.find(id);
  if (it != mAudioDevices.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Camera> ObjectRegistry::getCamera(SDL_CameraID id) const {
  auto it = mCameras.find(id);
  if (it != mCameras.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Gamepad> ObjectRegistry::getGamepad(SDL_JoystickID id) const {
  auto it = mGamepads.find(id);
  if (it != mGamepads.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Joystick> ObjectRegistry::getJoystick(SDL_JoystickID id) const {
  auto it = mJoysticks.find(id);
  if (it != mJoysticks.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Mouse> ObjectRegistry::getMouse(SDL_MouseID id) const {
  auto it = mMice.find(id);
  if (it != mMice.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Window> ObjectRegistry::getWindow(SDL_WindowID id) const {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
    return it->second;
  }
  return nullptr;
}

void ObjectRegistry::registerAudioDevice(SDL_AudioDeviceID id,
                                         shared<AudioDevice> device) {
  if (this->hasAudioDevice(id)) {
    throw Exception::SDLModuleError(
        "Audio device with ID " + std::to_string(id) + " already registered.");
  }
  mAudioDevices[id] = device;
}

void ObjectRegistry::unregisterAudioDevice(SDL_AudioDeviceID id) {
  auto it = mAudioDevices.find(id);
  if (it != mAudioDevices.end()) {
    mAudioDevices.erase(it);
  } else {
    throw Exception::SDLModuleError("Audio device with ID " +
                                    std::to_string(id) + " not found.");
  }
}

void ObjectRegistry::registerCamera(SDL_CameraID id, shared<Camera> camera) {
  if (this->hasCamera(id)) {
    throw Exception::SDLModuleError("Camera with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mCameras[id] = camera;
}

void ObjectRegistry::unregisterCamera(SDL_CameraID id) {
  auto it = mCameras.find(id);
  if (it != mCameras.end()) {
    mCameras.erase(it);
  } else {
    throw Exception::SDLModuleError("Camera with ID " + std::to_string(id) +
                                    " not found.");
  }
}

void ObjectRegistry::registerGamepad(SDL_JoystickID id,
                                     shared<Gamepad> gamepad) {
  if (this->hasGamepad(id)) {
    throw Exception::SDLModuleError("Gamepad with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mGamepads[id] = gamepad;
}

void ObjectRegistry::unregisterGamepad(SDL_JoystickID id) {
  auto it = mGamepads.find(id);
  if (it != mGamepads.end()) {
    mGamepads.erase(it);
  } else {
    throw Exception::SDLModuleError("Gamepad with ID " + std::to_string(id) +
                                    " not found.");
  }
}

void ObjectRegistry::registerJoystick(SDL_JoystickID id,
                                      shared<Joystick> joystick) {
  if (this->hasJoystick(id)) {
    throw Exception::SDLModuleError("Joystick with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mJoysticks[id] = joystick;
}

void ObjectRegistry::unregisterJoystick(SDL_JoystickID id) {
  auto it = mJoysticks.find(id);
  if (it != mJoysticks.end()) {
    mJoysticks.erase(it);
  } else {
    throw Exception::SDLModuleError("Joystick with ID " + std::to_string(id) +
                                    " not found.");
  }
}

void ObjectRegistry::registerMouse(SDL_MouseID id, shared<Mouse> mouse) {
  if (this->hasMouse(id)) {
    throw Exception::SDLModuleError("Mouse with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mMice[id] = mouse;
}

void ObjectRegistry::unregisterMouse(SDL_MouseID id) {
  auto it = mMice.find(id);
  if (it != mMice.end()) {
    mMice.erase(it);
  } else {
    throw Exception::SDLModuleError("Mouse with ID " + std::to_string(id) +
                                    " not found.");
  }
}

void ObjectRegistry::registerWindow(SDL_WindowID id, shared<Window> window) {
  if (this->hasWindow(id)) {
    throw Exception::SDLModuleError("Window with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mWindows[id] = window;
}

void ObjectRegistry::unregisterWindow(SDL_WindowID id) {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
    mWindows.erase(it);
  } else {
    throw Exception::SDLModuleError("Window with ID " + std::to_string(id) +
                                    " not found.");
  }
}
} // namespace Terreate::SDL
