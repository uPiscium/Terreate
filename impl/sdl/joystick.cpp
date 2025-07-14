#include <sdl/exception.hpp>
#include <sdl/joystick.hpp>

namespace Terreate::SDL {
Joystick::Joystick(u32 const &index) {
  mJoystickID = index;
  mJoystick = SDL_OpenJoystick(index);
  if (!mJoystick) {
    throw Exception::SDLModuleError("Failed to open joystick: " +
                                    str(SDL_GetError()));
  }
}

Joystick::~Joystick() {
  if (mJoystick) {
    SDL_CloseJoystick(mJoystick);
    mJoystick = nullptr;
  }
}

vec2 Joystick::getBall(i32 const &ball) const {
  i32 xrel = 0, yrel = 0;
  if (SDL_GetJoystickBall(mJoystick, ball, &xrel, &yrel) != 0) {
    throw Exception::SDLModuleError("Failed to get joystick ball: " +
                                    str(SDL_GetError()));
  }
  return {static_cast<float>(xrel), static_cast<float>(yrel)};
}

void Joystick::loadMapping(str const &mapping) {
  if (SDL_AddGamepadMapping(mapping.c_str()) != 0) {
    throw Exception::SDLModuleError("Failed to load joystick mapping: " +
                                    str(SDL_GetError()));
  }
}
} // namespace Terreate::SDL
