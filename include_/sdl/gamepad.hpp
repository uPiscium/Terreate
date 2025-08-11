#pragma once

#include "../common/enum.hpp"
#include "../common/math.hpp"
#include "../common/type.hpp"

#include "joystick.hpp"

#include <limits>

namespace Terreate::SDL {
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

  GUID getGUID() const { return SDL_GetGamepadGUIDForID(mGamepadID); }
  SDL_JoystickID getID() const { return mGamepadID; }
  str getName() const {
    return SDL_GetGamepadName(mGamepad) ? SDL_GetGamepadName(mGamepad) : "";
  }
  i32 getNumTouchpads() const { return SDL_GetNumGamepadTouchpads(mGamepad); }
  GamepadType getType() const {
    return (GamepadType)SDL_GetGamepadType(mGamepad);
  }
  i16 getRawAxis(GamepadAxis const &axis) const {
    return SDL_GetGamepadAxis(mGamepad, (SDL_GamepadAxis)axis);
  }
  float getAxis(GamepadAxis const &axis) const {
    return this->getRawAxis(axis) / (float)std::numeric_limits<i16>::max();
  }
  float getSensorDataRate(SDLSensor sensor) const {
    return SDL_GetGamepadSensorDataRate(mGamepad, (SDL_SensorType)sensor);
  }
  vec3 getSensorData(SDLSensor sensor) const;
  vec<Finger> getTouchpadFingers(u32 const &touchpadIndex) const;

  void setLEDColor(u8 const &red, u8 const &green, u8 const &blue) const {
    SDL_SetGamepadLED(mGamepad, red, green, blue);
  }
  void setPlayerIndex(i32 const &index) const {
    SDL_SetGamepadPlayerIndex(mGamepad, index);
  }

  bool isConnected() const { return SDL_GamepadConnected(mGamepad); }
  bool isPressed(GamepadButton const &button) const {
    return SDL_GetGamepadButton(mGamepad, (SDL_GamepadButton)button);
  }

  bool hasSensor(SDLSensor sensor) const {
    return SDL_GamepadHasSensor(mGamepad, (SDL_SensorType)sensor);
  }
  bool hasRumble() const {
    return SDL_RumbleGamepad(mGamepad, 0.0f, 0.0f, 0.0f);
  }
  bool hasRumbleTriggers() const {
    return SDL_RumbleGamepadTriggers(mGamepad, 0.0f, 0.0f, 0.0f);
  }
  bool hasLED() const { return SDL_SetGamepadLED(mGamepad, 0, 0, 0); }

  void rumble(float lowFrequency, float highFrequency, float duration) const {
    SDL_RumbleGamepad(mGamepad, lowFrequency, highFrequency, duration);
  }
  void rumbleTriggers(float leftTrigger, float rightTrigger,
                      float duration) const {
    SDL_RumbleGamepadTriggers(mGamepad, leftTrigger, rightTrigger, duration);
  }

public:
  static u32 getNumGamepads();
  static void loadMapping(str const &mapping) {
    Joystick::loadMapping(mapping);
  }
};

} // namespace Terreate::SDL
