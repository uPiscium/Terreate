#pragma once

#include "opengl/buffer.hpp"
#include "opengl/compute.hpp"
#include "opengl/converter.hpp"
#include "opengl/cursor.hpp"
#include "opengl/font.hpp"
#include "opengl/icon.hpp"
#include "opengl/joystick.hpp"
#include "opengl/mouse.hpp"
#include "opengl/screen.hpp"
#include "opengl/shader.hpp"
#include "opengl/text.hpp"
#include "opengl/texture.hpp"
#include "opengl/window.hpp"
#include "opengl/winhelper.hpp"

namespace Terreate::OpenGL {
void initializeGLFW();
void initializeGLAD();
void terminate();
} // namespace Terreate::OpenGL
