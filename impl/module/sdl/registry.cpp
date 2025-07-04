#include "../../../include/module/sdl/exception.hpp"
#include "../../../include/module/sdl/registry.hpp"

namespace Terreate::SDL {
shared<Window> SDLObjectRegistry::getWindow(SDL_WindowID id) const {
  auto it = mWindows.find(id);
  if (it != mWindows.end()) {
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
} // namespace Terreate::SDL
