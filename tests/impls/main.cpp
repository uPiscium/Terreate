#include "../include/main.hpp"

namespace Terreate::TestImpl {
GLFWwindow *VulkanTriangle::initWindow(int width, int height,
                                       Type::str const &title) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window =
      glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  return window;
}

void VulkanTriangle::destroyWindow(GLFWwindow *window) {
  if (window) {
    glfwDestroyWindow(window);
    glfwTerminate();
  }
}

void VulkanTriangle::initVulkan() {
  mWindow = this->initWindow();
  mInstance = this->createInstance();

  if (ENABLE_DEBUG_BUILD) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    this->populateDebugMessengerCreateInfo(createInfo);
    mDebugMessenger = this->createDebugMessenger(mInstance, createInfo);
  }
}

void VulkanTriangle::mainLoop() {
  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();
  }
}

void VulkanTriangle::cleanup() {
  if (ENABLE_DEBUG_BUILD) {
    this->destroyDebugMessenger(mInstance, mDebugMessenger);
  }

  this->destroyInstance(mInstance);
  this->destroyWindow(mWindow);
}

void VulkanTriangle::run() {
  try {
    this->initVulkan();
    this->mainLoop();
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    this->cleanup();
    throw; // Re-throw to handle in main
  }

  this->cleanup();
}
} // namespace Terreate::TestImpl

int main() {
  try {
    Terreate::TestImpl::VulkanTriangle app;
    app.run();
  } catch (const std::runtime_error &e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
