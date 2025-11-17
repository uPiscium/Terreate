#include "core/sdl/exception.hpp"
#include "core/sdl/gamepad.hpp"

namespace Terreate::SDL {

Gamepad::Gamepad(u32 const &index) {
  mGamepadID = index;
  mGamepad = SDL_OpenGamepad(index);
  if (!mGamepad) {
    throw GamepadException("Failed to open gamepad: " +
                           std::string(SDL_GetError()));
  }
}

Gamepad::~Gamepad() {
  if (mGamepad) {
    SDL_CloseGamepad(mGamepad);
    mGamepad = nullptr;
  }
}

GUID Gamepad::getGUID() const { return SDL_GetGamepadGUIDForID(mGamepadID); }

SDL_JoystickID Gamepad::getID() const { return mGamepadID; }

str Gamepad::getName() const {
  return SDL_GetGamepadName(mGamepad) ? SDL_GetGamepadName(mGamepad) : "";
}

i32 Gamepad::getNumTouchpads() const {
  return SDL_GetNumGamepadTouchpads(mGamepad);
}

GamepadType Gamepad::getType() const {
  return (GamepadType)SDL_GetGamepadType(mGamepad);
}

i16 Gamepad::getRawAxis(GamepadAxis const &axis) const {
  return SDL_GetGamepadAxis(mGamepad, (SDL_GamepadAxis)axis);
}

float Gamepad::getAxis(GamepadAxis const &axis) const {
  return this->getRawAxis(axis) / (float)std::numeric_limits<i16>::max();
}

float Gamepad::getSensorDataRate(SDLSensor sensor) const {
  return SDL_GetGamepadSensorDataRate(mGamepad, (SDL_SensorType)sensor);
}

vec3 Gamepad::getSensorData(SDLSensor sensor) const {
  if (!this->hasSensor(sensor)) {
    return {};
  }
  float data[3] = {0.0f, 0.0f, 0.0f};
  bool success =
      SDL_GetGamepadSensorData(mGamepad, (SDL_SensorType)sensor, data, 3);
  if (!success) {
    return {};
  }
  return vec3(data[0], data[1], data[2]);
}

vec<Finger> Gamepad::getTouchpadFingers(u32 const &touchpadIndex) const {
  i32 numFingers = SDL_GetNumGamepadTouchpadFingers(mGamepad, touchpadIndex);
  if (numFingers < 1) {
    return {};
  }
  vec<Finger> fingers(numFingers);
  for (i32 i = 0; i < numFingers; ++i) {
    if (!SDL_GetGamepadTouchpadFinger(mGamepad, touchpadIndex, i,
                                      &fingers[i].down, &fingers[i].position.x,
                                      &fingers[i].position.y,
                                      &fingers[i].pressure)) {
      fingers[i].position = {0.0f, 0.0f};
      fingers[i].pressure = 0.0f;
    }
  }
  return fingers;
}

u32 Gamepad::getNumGamepads() {
  int count = 0;
  SDL_GetGamepads(&count);
  return count;
}

void Gamepad::setLEDColor(u8 const &red, u8 const &green,
                          u8 const &blue) const {
  SDL_SetGamepadLED(mGamepad, red, green, blue);
}

void Gamepad::setPlayerIndex(i32 const &index) const {
  SDL_SetGamepadPlayerIndex(mGamepad, index);
}

bool Gamepad::isConnected() const { return SDL_GamepadConnected(mGamepad); }

bool Gamepad::isPressed(GamepadButton const &button) const {
  return SDL_GetGamepadButton(mGamepad, (SDL_GamepadButton)button);
}

bool Gamepad::hasSensor(SDLSensor sensor) const {
  return SDL_GamepadHasSensor(mGamepad, (SDL_SensorType)sensor);
}

bool Gamepad::hasRumble() const {
  return SDL_RumbleGamepad(mGamepad, 0.0f, 0.0f, 0.0f);
}

bool Gamepad::hasRumbleTriggers() const {
  return SDL_RumbleGamepadTriggers(mGamepad, 0.0f, 0.0f, 0.0f);
}

bool Gamepad::hasLED() const { return SDL_SetGamepadLED(mGamepad, 0, 0, 0); }

void Gamepad::rumble(float lowFrequency, float highFrequency,
                     float duration) const {
  SDL_RumbleGamepad(mGamepad, lowFrequency, highFrequency, duration);
}

void Gamepad::rumbleTriggers(float leftTrigger, float rightTrigger,
                             float duration) const {
  SDL_RumbleGamepadTriggers(mGamepad, leftTrigger, rightTrigger, duration);
}

void Gamepad::loadMapping(str const &mapping) {
  Joystick::loadMapping(mapping);
}

} // namespace Terreate::SDL
