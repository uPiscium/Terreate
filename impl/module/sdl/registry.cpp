#include "../../../include/module/sdl/exception.hpp"
#include "../../../include/module/sdl/registry.hpp"

namespace Terreate::SDL {
shared<AudioDevice>
SDLObjectRegistry::getAudioDevice(SDL_AudioDeviceID id) const {
  auto it = mAudioDevices.find(id);
  if (it != mAudioDevices.end()) {
    return it->second;
  }
  return nullptr;
}

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

void SDLObjectRegistry::registerAudioDevice(SDL_AudioDeviceID id,
                                            shared<AudioDevice> device) {
  if (mAudioDevices.find(id) != mAudioDevices.end()) {
    throw Exception::SDLModuleError(
        "Audio device with ID " + std::to_string(id) + " already registered.");
  }
  mAudioDevices[id] = device;
}

void SDLObjectRegistry::unregisterAudioDevice(SDL_AudioDeviceID id) {
  auto it = mAudioDevices.find(id);
  if (it != mAudioDevices.end()) {
    mAudioDevices.erase(it);
  } else {
    throw Exception::SDLModuleError("Audio device with ID " +
                                    std::to_string(id) + " not found.");
  }
}

void SDLObjectRegistry::registerGamepad(SDL_JoystickID id,
                                        shared<Gamepad> gamepad) {
  if (mGamepads.find(id) != mGamepads.end()) {
    throw Exception::SDLModuleError("Gamepad with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mGamepads[id] = gamepad;
}

void SDLObjectRegistry::unregisterGamepad(SDL_JoystickID id) {
  auto it = mGamepads.find(id);
  if (it != mGamepads.end()) {
    mGamepads.erase(it);
  } else {
    throw Exception::SDLModuleError("Gamepad with ID " + std::to_string(id) +
                                    " not found.");
  }
}

void SDLObjectRegistry::registerJoystick(SDL_JoystickID id,
                                         shared<Joystick> joystick) {
  if (mJoysticks.find(id) != mJoysticks.end()) {
    throw Exception::SDLModuleError("Joystick with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mJoysticks[id] = joystick;
}

void SDLObjectRegistry::unregisterJoystick(SDL_JoystickID id) {
  auto it = mJoysticks.find(id);
  if (it != mJoysticks.end()) {
    mJoysticks.erase(it);
  } else {
    throw Exception::SDLModuleError("Joystick with ID " + std::to_string(id) +
                                    " not found.");
  }
}

void SDLObjectRegistry::registerMouse(SDL_MouseID id, shared<Mouse> mouse) {
  if (mMice.find(id) != mMice.end()) {
    throw Exception::SDLModuleError("Mouse with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mMice[id] = mouse;
}

void SDLObjectRegistry::unregisterMouse(SDL_MouseID id) {
  auto it = mMice.find(id);
  if (it != mMice.end()) {
    mMice.erase(it);
  } else {
    throw Exception::SDLModuleError("Mouse with ID " + std::to_string(id) +
                                    " not found.");
  }
}

void SDLObjectRegistry::registerWindow(SDL_WindowID id, shared<Window> window) {
  if (mWindows.find(id) != mWindows.end()) {
    throw Exception::SDLModuleError("Window with ID " + std::to_string(id) +
                                    " already registered.");
  }
  mWindows[id] = window;
}

void SDLObjectRegistry::unregisterWindow(SDL_WindowID id) {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
    mWindows.erase(it);
  } else {
    throw Exception::SDLModuleError("Window with ID " + std::to_string(id) +
                                    " not found.");
  }
}
} // namespace Terreate::SDL
