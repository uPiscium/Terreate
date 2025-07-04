#include "../../include/module/sdl.hpp"

namespace Terreate::SDL {
void initializeSDL() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_CAMERA |
                SDL_INIT_SENSOR | SDL_INIT_AUDIO)) {
    throw Exception::SDLModuleError("Failed to initialize SDL: " +
                                    std::string(SDL_GetError()));
    return;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void initializeGLAD() {
  if (!gladLoaderLoadGL()) {
    throw Exception::SDLModuleError("Failed to initialize GLAD");
    return;
  }
}

void terminate() {
  gladLoaderUnloadGL();
  SDL_Quit();
}
} // namespace Terreate::SDL
