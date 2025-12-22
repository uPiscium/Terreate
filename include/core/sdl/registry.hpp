#pragma once

#include "common/type.hpp"

#include "audio.hpp"
#include "camera.hpp"
#include "gamepad.hpp"
#include "joystick.hpp"
#include "mouse.hpp"
#include "window.hpp"

namespace Terreate::Core::SDL {

class Registry {
private:
  PROHIBIT_COPY_AND_ASSIGN(Registry);

private:
  umap<AudioDeviceID, shared<AudioDevice>> mAudioDevices;
  umap<CameraID, shared<Camera>> mCameras;
  umap<JoystickID, shared<Gamepad>> mGamepads;
  umap<JoystickID, shared<Joystick>> mJoysticks;
  umap<MouseID, shared<Mouse>> mMice;
  umap<WindowID, shared<Window>> mWindows;

public:
  Registry() = default;
  ~Registry() = default;

  shared<AudioDevice> getAudioDevice(AudioDeviceID id) const;
  shared<Camera> getCamera(CameraID id) const;
  shared<Gamepad> getGamepad(JoystickID id) const;
  shared<Joystick> getJoystick(JoystickID id) const;
  shared<Mouse> getMouse(MouseID id) const;
  shared<Window> getWindow(WindowID id) const;

  bool hasAudioDevice(AudioDeviceID id) const;
  bool hasCamera(CameraID id) const;
  bool hasGamepad(JoystickID id) const;
  bool hasJoystick(JoystickID id) const;
  bool hasMouse(MouseID id) const;
  bool hasWindow(WindowID id) const;

  void registerAudioDevice(AudioDeviceID id, shared<AudioDevice> device);
  void unregisterAudioDevice(AudioDeviceID id);
  void registerCamera(CameraID id, shared<Camera> camera);
  void unregisterCamera(CameraID id);
  void registerGamepad(JoystickID id, shared<Gamepad> gamepad);
  void unregisterGamepad(JoystickID id);
  void registerJoystick(JoystickID id, shared<Joystick> joystick);
  void unregisterJoystick(JoystickID id);
  void registerMouse(MouseID id, shared<Mouse> mouse);
  void unregisterMouse(MouseID id);
  void registerWindow(WindowID id, shared<Window> window);
  void unregisterWindow(WindowID id);
};

} // namespace Terreate::Core::SDL
