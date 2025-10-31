#include "sdl/exception.hpp"
#include "sdl/registry.hpp"

namespace Terreate::SDL {

// bool SDLObjectRegistry::hasAudioDevice(SDL_AudioDeviceID id) const {
//   return mAudioDevices.contains(id);
// }

// bool SDLObjectRegistry::hasCamera(SDL_CameraID id) const {
//   return mCameras.contains(id);
// }

bool SDLObjectRegistry::hasGamepad(SDL_JoystickID id) const {
  return mGamepads.contains(id);
}

bool SDLObjectRegistry::hasJoystick(SDL_JoystickID id) const {
  return mJoysticks.contains(id);
}

bool SDLObjectRegistry::hasMouse(SDL_MouseID id) const {
  return mMice.contains(id);
}

bool SDLObjectRegistry::hasWindow(SDL_WindowID id) const {
  return mWindows.contains(id);
}

// shared<AudioDevice> ObjectRegistry::getAudioDevice(SDL_AudioDeviceID id)
// const {
//   auto it = mAudioDevices.find(id);
//   if (it != mAudioDevices.end()) {
//     return it->second;
//   }
//   return nullptr;
// }

// shared<Camera> ObjectRegistry::getCamera(SDL_CameraID id) const {
//   auto it = mCameras.find(id);
//   if (it != mCameras.end()) {
//     return it->second;
//   }
//   return nullptr;
// }

shared<Gamepad> SDLObjectRegistry::getGamepad(SDL_JoystickID id) const {
  auto it = mGamepads.find(id);
  if (it != mGamepads.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Joystick> SDLObjectRegistry::getJoystick(SDL_JoystickID id) const {
  auto it = mJoysticks.find(id);
  if (it != mJoysticks.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Mouse> SDLObjectRegistry::getMouse(SDL_MouseID id) const {
  auto it = mMice.find(id);
  if (it != mMice.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Window> SDLObjectRegistry::getWindow(SDL_WindowID id) const {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
    return it->second;
  }
  return nullptr;
}

// void SDLObjectRegistry::registerAudioDevice(SDL_AudioDeviceID id,
//                                             shared<AudioDevice> device) {
//   if (this->hasAudioDevice(id)) {
//     throw Exception::SDLModuleError(
//         "Audio device with ID " + std::to_string(id) + " already
//         registered.");
//   }
//   mAudioDevices[id] = device;
// }

// void SDLObjectRegistry::unregisterAudioDevice(SDL_AudioDeviceID id) {
//   auto it = mAudioDevices.find(id);
//   if (it != mAudioDevices.end()) {
//     mAudioDevices.erase(it);
//   } else {
//     throw Exception::SDLModuleError("Audio device with ID " +
//                                     std::to_string(id) + " not found.");
//   }
// }

// void SDLObjectRegistry::registerCamera(SDL_CameraID id, shared<Camera>
// camera) {
//   if (this->hasCamera(id)) {
//     throw Exception::SDLModuleError("Camera with ID " + std::to_string(id) +
//                                     " already registered.");
//   }
//   mCameras[id] = camera;
// }

// void SDLObjectRegistry::unregisterCamera(SDL_CameraID id) {
//   auto it = mCameras.find(id);
//   if (it != mCameras.end()) {
//     mCameras.erase(it);
//   } else {
//     throw Exception::SDLModuleError("Camera with ID " + std::to_string(id) +
//                                     " not found.");
//   }
// }

void SDLObjectRegistry::registerGamepad(SDL_JoystickID id,
                                        shared<Gamepad> gamepad) {
  if (this->hasGamepad(id)) {
    throw RegistryException("Gamepad with ID " + std::to_string(id) +
                            " already registered.");
  }
  mGamepads[id] = gamepad;
}

void SDLObjectRegistry::unregisterGamepad(SDL_JoystickID id) {
  auto it = mGamepads.find(id);
  if (it != mGamepads.end()) {
    mGamepads.erase(it);
  } else {
    throw RegistryException("Gamepad with ID " + std::to_string(id) +
                            " not found.");
  }
}

void SDLObjectRegistry::registerJoystick(SDL_JoystickID id,
                                         shared<Joystick> joystick) {
  if (this->hasJoystick(id)) {
    throw RegistryException("Joystick with ID " + std::to_string(id) +
                            " already registered.");
  }
  mJoysticks[id] = joystick;
}

void SDLObjectRegistry::unregisterJoystick(SDL_JoystickID id) {
  auto it = mJoysticks.find(id);
  if (it != mJoysticks.end()) {
    mJoysticks.erase(it);
  } else {
    throw RegistryException("Joystick with ID " + std::to_string(id) +
                            " not found.");
  }
}

void SDLObjectRegistry::registerMouse(SDL_MouseID id, shared<Mouse> mouse) {
  if (this->hasMouse(id)) {
    throw RegistryException("Mouse with ID " + std::to_string(id) +
                            " already registered.");
  }
  mMice[id] = mouse;
}

void SDLObjectRegistry::unregisterMouse(SDL_MouseID id) {
  auto it = mMice.find(id);
  if (it != mMice.end()) {
    mMice.erase(it);
  } else {
    throw RegistryException("Mouse with ID " + std::to_string(id) +
                            " not found.");
  }
}

void SDLObjectRegistry::registerWindow(SDL_WindowID id, shared<Window> window) {
  if (this->hasWindow(id)) {
    throw RegistryException("Window with ID " + std::to_string(id) +
                            " already registered.");
  }
  mWindows[id] = window;
}

void SDLObjectRegistry::unregisterWindow(SDL_WindowID id) {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
    mWindows.erase(it);
  } else {
    throw RegistryException("Window with ID " + std::to_string(id) +
                            " not found.");
  }
}

} // namespace Terreate::SDL
