#pragma once

#include "../common/math.hpp"
#include "../common/type.hpp"

#include <limits>

namespace Terreate::SDL {
class Joystick {
private:
  PROHIBIT_COPY_AND_ASSIGN(Joystick);

private:
  SDL_JoystickID mJoystickID = 0;
  SDL_Joystick *mJoystick = nullptr;

public:
  Joystick(u32 const &index);
  ~Joystick();

  u32 getID() const { return mJoystickID; }
  str getName() const {
    return SDL_GetJoystickName(mJoystick) ? SDL_GetJoystickName(mJoystick) : "";
  }
  i32 getNumAxes() const { return SDL_GetNumJoystickAxes(mJoystick); }
  i32 getNumBalls() const { return SDL_GetNumJoystickBalls(mJoystick); }
  i32 getNumHats() const { return SDL_GetNumJoystickHats(mJoystick); }
  i32 getNumButtons() const { return SDL_GetNumJoystickButtons(mJoystick); }
  i16 getRawAxis(i32 const &axis) const {
    return SDL_GetJoystickAxis(mJoystick, axis);
  }
  float getAxis(i32 const &axis) const {
    return this->getRawAxis(axis) / (float)std::numeric_limits<i16>::max();
  }
  vec2 getBall(i32 const &ball) const;
  u8 getHat(i32 const &hat) const { return SDL_GetJoystickHat(mJoystick, hat); }

  void setLEDColor(u8 const &red, u8 const &green, u8 const &blue) const {
    SDL_SetJoystickLED(mJoystick, red, green, blue);
  }
  void setPlayerIndex(i32 const &index) const {
    SDL_SetJoystickPlayerIndex(mJoystick, index);
  }

  bool isButtonPressed(i32 const &button) const {
    return SDL_GetJoystickButton(mJoystick, button);
  }
  bool isGamepad() const { return SDL_IsGamepad(mJoystickID); }
  bool isConnected() const { return SDL_JoystickConnected(mJoystick); }

public:
  static void loadMapping(str const &mapping);
};
} // namespace Terreate::SDL
