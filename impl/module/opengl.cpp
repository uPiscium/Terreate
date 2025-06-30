#include "../../include/module/opengl.hpp"

namespace Terreate::OpenGL {
void initializeGLFW() {
  if (!glfwInit()) {
    throw Exception::GraphicModuleError("Failed to initialize GLFW");
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

  glfwSetJoystickCallback(joystickCallback);
}

void initializeGLAD() {
  if (!gladLoaderLoadGL()) {
    throw Exception::GraphicModuleError("Failed to initialize GLAD");
    return;
  }
}

void terminate() {
  gladLoaderUnloadGL();
  glfwTerminate();
}
} // namespace Terreate::OpenGL
