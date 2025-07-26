#pragma once

#include "../common/type.hpp"

#include "audio.hpp"
#include "camera.hpp"
#include "gamepad.hpp"
#include "joystick.hpp"
#include "mouse.hpp"
#include "window.hpp"

namespace Terreate::SDL {
class ObjectRegistry {
private:
  PROHIBIT_COPY_AND_ASSIGN(ObjectRegistry);

private:
  umap<SDL_AudioDeviceID, shared<AudioDevice>> mAudioDevices;
  umap<SDL_CameraID, shared<Camera>> mCameras;
  umap<SDL_JoystickID, shared<Gamepad>> mGamepads;
  umap<SDL_JoystickID, shared<Joystick>> mJoysticks;
  umap<SDL_MouseID, shared<Mouse>> mMice;
  umap<SDL_WindowID, shared<Window>> mWindows;

public:
  ObjectRegistry() = default;
  ~ObjectRegistry() = default;

  shared<AudioDevice> getAudioDevice(SDL_AudioDeviceID id) const;
  shared<Camera> getCamera(SDL_CameraID id) const;
  shared<Gamepad> getGamepad(SDL_JoystickID id) const;
  shared<Joystick> getJoystick(SDL_JoystickID id) const;
  shared<Mouse> getMouse(SDL_MouseID id) const;
  shared<Window> getWindow(SDL_WindowID id) const;

  bool hasAudioDevice(SDL_AudioDeviceID id) const {
    return mAudioDevices.contains(id);
  }
  bool hasCamera(SDL_CameraID id) const { return mCameras.contains(id); }
  bool hasGamepad(SDL_JoystickID id) const { return mGamepads.contains(id); }
  bool hasJoystick(SDL_JoystickID id) const { return mJoysticks.contains(id); }
  bool hasMouse(SDL_MouseID id) const { return mMice.contains(id); }
  bool hasWindow(SDL_WindowID id) const { return mWindows.contains(id); }

  void registerAudioDevice(SDL_AudioDeviceID id, shared<AudioDevice> device);
  void unregisterAudioDevice(SDL_AudioDeviceID id);
  void registerCamera(SDL_CameraID id, shared<Camera> camera);
  void unregisterCamera(SDL_CameraID id);
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
