#pragma once

#include "../common/type.hpp"

#include "audio.hpp"
#include "gamepad.hpp"
#include "joystick.hpp"
#include "mouse.hpp"
#include "window.hpp"

namespace Terreate::SDL {
class SDLObjectRegistry {
private:
  PROHIBIT_COPY_AND_ASSIGN(SDLObjectRegistry);

private:
  umap<SDL_AudioDeviceID, shared<AudioDevice>> mAudioDevices;
  umap<SDL_JoystickID, shared<Gamepad>> mGamepads;
  umap<SDL_JoystickID, shared<Joystick>> mJoysticks;
  umap<SDL_MouseID, shared<Mouse>> mMice;
  umap<SDL_WindowID, shared<Window>> mWindows;

public:
  SDLObjectRegistry() = default;
  ~SDLObjectRegistry() = default;

  shared<AudioDevice> getAudioDevice(SDL_AudioDeviceID id) const;
  shared<Gamepad> getGamepad(SDL_JoystickID id) const;
  shared<Joystick> getJoystick(SDL_JoystickID id) const;
  shared<Mouse> getMouse(SDL_MouseID id) const;
  shared<Window> getWindow(SDL_WindowID id) const;

  bool hasAudioDevice(SDL_AudioDeviceID id) const {
    return mAudioDevices.find(id) != mAudioDevices.end();
  }
  bool hasGamepad(SDL_JoystickID id) const {
    return mGamepads.find(id) != mGamepads.end();
  }
  bool hasJoystick(SDL_JoystickID id) const {
    return mJoysticks.find(id) != mJoysticks.end();
  }
  bool hasMouse(SDL_MouseID id) const { return mMice.find(id) != mMice.end(); }
  bool hasWindow(SDL_WindowID id) const {
    return mWindows.find(id) != mWindows.end();
  }

  void registerAudioDevice(SDL_AudioDeviceID id, shared<AudioDevice> device);
  void unregisterAudioDevice(SDL_AudioDeviceID id);
  void registerGamepad(SDL_JoystickID id, shared<Gamepad> gamepad);
  void unregisterGamepad(SDL_JoystickID id);
  void registerJoystick(SDL_JoystickID id, shared<Joystick> joystick);
  void unregisterJoystick(SDL_JoystickID id);
  void registerMouse(SDL_MouseID id, shared<Mouse> mouse);
  void unregisterMouse(SDL_MouseID id);
  void registerWindow(SDL_WindowID id, shared<Window> window);
  void unregisterWindow(SDL_WindowID id);
};
} // namespace Terreate::SDL
