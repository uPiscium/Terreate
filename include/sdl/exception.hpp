#pragma once

#include "common/exception.hpp"

namespace Terreate::SDL {

class SDLException : public TerreateException {
public:
  SDLException(str const &message) : TerreateException(message) {}
};

class JoystickException : public SDLException {
public:
  JoystickException(str const &message) : SDLException(message) {}
};

class GamepadException : public SDLException {
public:
  GamepadException(str const &message) : SDLException(message) {}
};

class RegistryException : public SDLException {
public:
  RegistryException(str const &message) : SDLException(message) {}
};

class AudioException : public SDLException {
public:
  AudioException(str const &message) : SDLException(message) {}
};

} // namespace Terreate::SDL
