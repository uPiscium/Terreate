#include <context.hpp>

namespace Terreate {
TerreateContext::TerreateContext() {
  // Initialize GLFW
  if (!glfwInit()) {
    throw Exception::GLFWInitializeFailure();
  }

  // Set GLFW to not create an OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

TerreateContext::~TerreateContext() {
  // Terminate GLFW
  glfwTerminate();
}
} // namespace Terreate
