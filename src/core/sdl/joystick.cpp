#include "core/sdl/exception.hpp"
#include "core/sdl/joystick.hpp"

#include <limits>

namespace Terreate::SDL {

Joystick::Joystick(u32 const &index) {
  mJoystickID = index;
  mJoystick = SDL_OpenJoystick(index);
  if (!mJoystick) {
    throw JoystickException("Failed to open joystick: " + str(SDL_GetError()));
  }
}

Joystick::~Joystick() {
  if (mJoystick) {
    SDL_CloseJoystick(mJoystick);
    mJoystick = nullptr;
  }
}

u32 Joystick::getID() const { return mJoystickID; }

str Joystick::getName() const {
  return SDL_GetJoystickName(mJoystick) ? SDL_GetJoystickName(mJoystick) : "";
}

i32 Joystick::getNumAxes() const { return SDL_GetNumJoystickAxes(mJoystick); }

i32 Joystick::getNumBalls() const { return SDL_GetNumJoystickBalls(mJoystick); }

i32 Joystick::getNumHats() const { return SDL_GetNumJoystickHats(mJoystick); }

i32 Joystick::getNumButtons() const {
  return SDL_GetNumJoystickButtons(mJoystick);
}

i16 Joystick::getRawAxis(i32 const &axis) const {
  return SDL_GetJoystickAxis(mJoystick, axis);
}

float Joystick::getAxis(i32 const &axis) const {
  return this->getRawAxis(axis) / (float)std::numeric_limits<i16>::max();
}

vec2 Joystick::getBall(i32 const &ball) const {
  i32 xrel = 0, yrel = 0;
  if (SDL_GetJoystickBall(mJoystick, ball, &xrel, &yrel) != 0) {
    throw JoystickException("Failed to get joystick ball: " +
                            str(SDL_GetError()));
  }
  return {static_cast<float>(xrel), static_cast<float>(yrel)};
}

u8 Joystick::getHat(i32 const &hat) const {
  return SDL_GetJoystickHat(mJoystick, hat);
}

void Joystick::setLEDColor(u8 const &red, u8 const &green,
                           u8 const &blue) const {
  SDL_SetJoystickLED(mJoystick, red, green, blue);
}

void Joystick::setPlayerIndex(i32 const &index) const {
  SDL_SetJoystickPlayerIndex(mJoystick, index);
}

bool Joystick::isButtonPressed(i32 const &button) const {
  return SDL_GetJoystickButton(mJoystick, button);
}

bool Joystick::isGamepad() const { return SDL_IsGamepad(mJoystickID); }

bool Joystick::isConnected() const { return SDL_JoystickConnected(mJoystick); }

void Joystick::loadMapping(str const &mapping) {
  if (SDL_AddGamepadMapping(mapping.c_str()) != 0) {
    throw JoystickException("Failed to load joystick mapping: " +
                            str(SDL_GetError()));
  }
}

} // namespace Terreate::SDL
