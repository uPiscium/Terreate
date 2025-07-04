#pragma once

#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Exception {
class SDLModuleError : public TerreateException {
public:
  SDLModuleError(str const &message) : TerreateException(message) {}
};

class GamepadError : public SDLModuleError {
public:
  GamepadError(str const &message) : SDLModuleError(message) {}
};

class WindowError : public SDLModuleError {
public:
  WindowError(str const &message) : SDLModuleError(message) {}
};
} // namespace Terreate::Exception
