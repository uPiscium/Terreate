#pragma once

#include "common/math.hpp"
#include "common/type.hpp"

namespace Terreate::Core::SDL {

class Joystick {
private:
  PROHIBIT_COPY_AND_ASSIGN(Joystick);

private:
  SDL_JoystickID mJoystickID = 0;
  SDL_Joystick *mJoystick = nullptr;

public:
  Joystick(u32 const &index);
  ~Joystick();

  u32 getID() const;
  str getName() const;
  i32 getNumAxes() const;
  i32 getNumBalls() const;
  i32 getNumHats() const;
  i32 getNumButtons() const;
  i16 getRawAxis(i32 const &axis) const;
  float getAxis(i32 const &axis) const;
  vec2 getBall(i32 const &ball) const;
  u8 getHat(i32 const &hat) const;

  void setLEDColor(u8 const &red, u8 const &green, u8 const &blue) const;
  void setPlayerIndex(i32 const &index) const;

  bool isButtonPressed(i32 const &button) const;
  bool isGamepad() const;
  bool isConnected() const;

public:
  static void loadMapping(str const &mapping);
};

} // namespace Terreate::Core::SDL
