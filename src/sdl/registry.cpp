#include "sdl/exception.hpp"
#include "sdl/registry.hpp"

namespace Terreate::SDL {

bool Registry::hasAudioDevice(AudioDeviceID id) const {
  return mAudioDevices.contains(id);
}

bool Registry::hasCamera(CameraID id) const { return mCameras.contains(id); }

bool Registry::hasGamepad(JoystickID id) const {
  return mGamepads.contains(id);
}

bool Registry::hasJoystick(JoystickID id) const {
  return mJoysticks.contains(id);
}

bool Registry::hasMouse(MouseID id) const { return mMice.contains(id); }

bool Registry::hasWindow(WindowID id) const { return mWindows.contains(id); }

shared<AudioDevice> Registry::getAudioDevice(AudioDeviceID id) const {
  auto it = mAudioDevices.find(id);
  if (it != mAudioDevices.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Camera> Registry::getCamera(CameraID id) const {
  auto it = mCameras.find(id);
  if (it != mCameras.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Gamepad> Registry::getGamepad(JoystickID id) const {
  auto it = mGamepads.find(id);
  if (it != mGamepads.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Joystick> Registry::getJoystick(JoystickID id) const {
  auto it = mJoysticks.find(id);
  if (it != mJoysticks.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Mouse> Registry::getMouse(MouseID id) const {
  auto it = mMice.find(id);
  if (it != mMice.end()) {
    return it->second;
  }
  return nullptr;
}

shared<Window> Registry::getWindow(WindowID id) const {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
    return it->second;
  }
  return nullptr;
}

void Registry::registerAudioDevice(AudioDeviceID id,
                                   shared<AudioDevice> device) {
  if (this->hasAudioDevice(id)) {
    throw RegistryException("Audio device with ID " + std::to_string(id) +
                            " already registered.");
  }
  mAudioDevices[id] = device;
}

void Registry::unregisterAudioDevice(AudioDeviceID id) {
  auto it = mAudioDevices.find(id);
  if (it != mAudioDevices.end()) {
    mAudioDevices.erase(it);
  } else {
    throw RegistryException("Audio device with ID " + std::to_string(id) +
                            " not found.");
  }
}

void Registry::registerCamera(CameraID id, shared<Camera> camera) {
  if (this->hasCamera(id)) {
    throw RegistryException("Camera with ID " + std::to_string(id) +
                            " already registered.");
  }
  mCameras[id] = camera;
}

void Registry::unregisterCamera(CameraID id) {
  auto it = mCameras.find(id);
  if (it != mCameras.end()) {
    mCameras.erase(it);
  } else {
    throw RegistryException("Camera with ID " + std::to_string(id) +
                            " not found.");
  }
}

void Registry::registerGamepad(JoystickID id, shared<Gamepad> gamepad) {
  if (this->hasGamepad(id)) {
    throw RegistryException("Gamepad with ID " + std::to_string(id) +
                            " already registered.");
  }
  mGamepads[id] = gamepad;
}

void Registry::unregisterGamepad(JoystickID id) {
  auto it = mGamepads.find(id);
  if (it != mGamepads.end()) {
    mGamepads.erase(it);
  } else {
    throw RegistryException("Gamepad with ID " + std::to_string(id) +
                            " not found.");
  }
}

void Registry::registerJoystick(JoystickID id, shared<Joystick> joystick) {
  if (this->hasJoystick(id)) {
    throw RegistryException("Joystick with ID " + std::to_string(id) +
                            " already registered.");
  }
  mJoysticks[id] = joystick;
}

void Registry::unregisterJoystick(JoystickID id) {
  auto it = mJoysticks.find(id);
  if (it != mJoysticks.end()) {
    mJoysticks.erase(it);
  } else {
    throw RegistryException("Joystick with ID " + std::to_string(id) +
                            " not found.");
  }
}

void Registry::registerMouse(MouseID id, shared<Mouse> mouse) {
  if (this->hasMouse(id)) {
    throw RegistryException("Mouse with ID " + std::to_string(id) +
                            " already registered.");
  }
  mMice[id] = mouse;
}

void Registry::unregisterMouse(MouseID id) {
  auto it = mMice.find(id);
  if (it != mMice.end()) {
    mMice.erase(it);
  } else {
    throw RegistryException("Mouse with ID " + std::to_string(id) +
                            " not found.");
  }
}

void Registry::registerWindow(WindowID id, shared<Window> window) {
  if (this->hasWindow(id)) {
    throw RegistryException("Window with ID " + std::to_string(id) +
                            " already registered.");
  }
  mWindows[id] = window;
}

void Registry::unregisterWindow(WindowID id) {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
    mWindows.erase(it);
  } else {
    throw RegistryException("Window with ID " + std::to_string(id) +
                            " not found.");
  }
}

} // namespace Terreate::SDL
