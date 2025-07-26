#pragma once

#include "audio.hpp"
#include "camera.hpp"
#include "event.hpp"
#include "exception.hpp"
#include "gamepad.hpp"
#include "icon.hpp"
#include "image.hpp"
#include "joystick.hpp"
#include "mouse.hpp"
#include "registry.hpp"
#include "window.hpp"
#include "winhelper.hpp"

namespace Terreate::SDL {
void initializeSDL();
void initializeGLAD();
void terminate();
} // namespace Terreate::SDL
