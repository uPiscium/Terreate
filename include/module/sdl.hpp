#pragma once

#include "sdl/cursor.hpp"
#include "sdl/exception.hpp"
#include "sdl/gamepad.hpp"
#include "sdl/icon.hpp"
#include "sdl/mouse.hpp"
#include "sdl/window.hpp"
#include "sdl/winhelper.hpp"

namespace Terreate::SDL {
void initializeSDL();
void initializeGLAD();
void terminate();
} // namespace Terreate::SDL
