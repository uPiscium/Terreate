#pragma once

#include "../common/type.hpp"

#include "gamepad.hpp"
#include "joystick.hpp"
#include "window.hpp"

namespace Terreate::SDL {
class SDLObjectRegistry {
private:
  umap<SDL_WindowID, shared<Window>> mWindows;
  umap<SDL_JoystickID, shared<Gamepad>> mGamepads;
  umap<SDL_JoystickID, shared<Joystick>> mJoysticks;

public:
  SDLObjectRegistry() = default;
  ~SDLObjectRegistry() = default;

  shared<Window> getWindow(SDL_WindowID id) const;
  shared<Gamepad> getGamepad(SDL_JoystickID id) const;
  shared<Joystick> getJoystick(SDL_JoystickID id) const;

  bool hasWindow(SDL_WindowID id) const {
    return mWindows.find(id) != mWindows.end();
  }
  bool hasGamepad(SDL_JoystickID id) const {
    return mGamepads.find(id) != mGamepads.end();
  }
  bool hasJoystick(SDL_JoystickID id) const {
    return mJoysticks.find(id) != mJoysticks.end();
  }

  void registerWindow(SDL_WindowID id, shared<Window> window);
  void unregisterWindow(SDL_WindowID id);
  void registerGamepad(SDL_JoystickID id, shared<Gamepad> gamepad);
  void unregisterGamepad(SDL_JoystickID id);
  void registerJoystick(SDL_JoystickID id, shared<Joystick> joystick);
  void unregisterJoystick(SDL_JoystickID id);
};
} // namespace Terreate::SDL
