#include "../../../include/module/sdl/exception.hpp"
#include "../../../include/module/sdl/gamepad.hpp"

namespace Terreate::SDL {
Gamepad::Gamepad(u32 const &index) {
  mGamepad = SDL_OpenGamepad(index);
  if (!mGamepad) {
    throw Exception::GamepadError("Failed to open gamepad: " +
                                  std::string(SDL_GetError()));
  }
}

Gamepad::~Gamepad() {
  if (mGamepad) {
    SDL_CloseGamepad(mGamepad);
    mGamepad = nullptr;
  }
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
} // namespace Terreate::SDL
