#pragma once

#include "common/math.hpp"
#include "common/type.hpp"

#include "enum.hpp"
#include "joystick.hpp"

namespace Terreate::Core::SDL {

struct Finger {
  bool down;
  vec2 position;
  float pressure;
};

class Gamepad {
private:
  PROHIBIT_COPY_AND_ASSIGN(Gamepad);

private:
  SDL_JoystickID mGamepadID;
  SDL_Gamepad *mGamepad = nullptr;

public:
  Gamepad(u32 const &index);
  Gamepad(Joystick const &joystick);
  ~Gamepad();

  GUID getGUID() const;
  SDL_JoystickID getID() const;
  str getName() const;
  i32 getNumTouchpads() const;
  GamepadType getType() const;
  i16 getRawAxis(GamepadAxis const &axis) const;
  float getAxis(GamepadAxis const &axis) const;
  float getSensorDataRate(SDLSensor sensor) const;
  vec3 getSensorData(SDLSensor sensor) const;
  vec<Finger> getTouchpadFingers(u32 const &touchpadIndex) const;

  void setLEDColor(u8 const &red, u8 const &green, u8 const &blue) const;
  void setPlayerIndex(i32 const &index) const;

  bool isConnected() const;
  bool isPressed(GamepadButton const &button) const;

  bool hasSensor(SDLSensor sensor) const;
  bool hasRumble() const;
  bool hasRumbleTriggers() const;
  bool hasLED() const;

  void rumble(float lowFrequency, float highFrequency, float duration) const;
  void rumbleTriggers(float leftTrigger, float rightTrigger,
                      float duration) const;

public:
  static u32 getNumGamepads();
  static void loadMapping(str const &mapping);
};

} // namespace Terreate::Core::SDL
