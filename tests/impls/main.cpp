#include <core/context.hpp>
#include <core/debugger.hpp>
#include <core/window.hpp>

#include "../include/_main.hpp"

#include <fstream>
#include <iostream>

using namespace Terreate;

class MyDebugger : public Core::IDebugger {
public:
  virtual bool verbose(Type::str const &message, Type::MessageType const type,
                       Type::vec<Core::DebugObject> const &object) override {
    // std::cout << "Verbose: " << message << std::endl;
    return false;
  }

  virtual bool info(Type::str const &message, Type::MessageType const type,
                    Type::vec<Core::DebugObject> const &object) override {
    // std::cout << "Info: " << message << std::endl;
    return false;
  }

  virtual bool warning(Type::str const &message, Type::MessageType const type,
                       Type::vec<Core::DebugObject> const &object) override {
    std::cout << "Warning: " << message << std::endl;
    return false;
  }

  virtual bool error(Type::str const &message, Type::MessageType const type,
                     Type::vec<Core::DebugObject> const &object) override {
    std::cout << "Error: " << message << std::endl;
    return false;
  }
};

Type::vec<Type::byte> readFile(std::string const &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  size_t fileSize = (size_t)file.tellg();
  Type::vec<Type::byte> buffer(fileSize);
  file.seekg(0);
  file.read((char *)buffer.data(), fileSize);
  file.close();
  return buffer;
}

int test1() {
  Core::Context ctx("Terreate", Type::Version(0, 1, 0));

  auto *debugger = new MyDebugger();
  ctx.attachDebugger(debugger);

  auto window = ctx.createWindow("Terreate", {800, 600},
                                 Core::WindowSettings{.resizable = false});
  auto surface = ctx.createSurface(window);
  auto swapchain = ctx.createSwapchain(window, surface);
  auto graphicQueue = ctx.createGraphicQueue();
  auto presentQueue = ctx.createPresentQueue();
  auto renderPass = ctx.createRenderPass(swapchain);
  auto pipeline = ctx.createPipeline(swapchain, renderPass);
  auto framebuffer = ctx.createFramebuffer(renderPass, swapchain);
  auto commandPool = ctx.createCommandPool();
  auto commandBuffer = commandPool->createCommandBuffer();
  auto semaphoreImageAvailable = ctx.createSemaphore();
  auto semaphoreRenderFinished = ctx.createSemaphore();
  auto fenceInFlight = ctx.createFence();

  auto vertShaderCode = readFile("tests/resources/shader/vert.spv");
  auto fragShaderCode = readFile("tests/resources/shader/frag.spv");

  pipeline->attachCompiledShaderSources(vertShaderCode, fragShaderCode);

  while (!window->isClosed()) {
    glfwPollEvents();

    fenceInFlight->wait();
    fenceInFlight->reset();
    Type::u32 imageIndex = pipeline->getNextImageIndex(semaphoreImageAvailable);

    commandBuffer->reset();
    commandBuffer->begin();
    commandBuffer->setRenderPass(renderPass, swapchain, pipeline,
                                 (*framebuffer)[imageIndex], {0, 0, 0, 1},
                                 Type::SubpassContent::INLINE);
    auto framebufferSize = swapchain->getProperty().extent;
    commandBuffer->setViewport(0, 0, framebufferSize.width,
                               framebufferSize.height, 0.0f, 1.0f);
    commandBuffer->setScissor(0, 0, framebufferSize.width,
                              framebufferSize.height);
    commandBuffer->drawBuffer(3, 1, 0, 0); // Draw a triangle
    commandBuffer->end();

    graphicQueue->queue({commandBuffer},
                        {Type::PipelineStage::COLOR_ATTACHMENT_OUTPUT_BIT},
                        {semaphoreImageAvailable}, {semaphoreRenderFinished});
    graphicQueue->submit(fenceInFlight);
    presentQueue->present(swapchain, {imageIndex}, semaphoreRenderFinished);
  }

  delete debugger;

  return 0;
}

void test2() {
#ifdef TERREATE_DEBUG_BUILD
  std::cout << "Running in debug mode." << std::endl;
#else
  std::cout << "Running in release mode." << std::endl;
#endif

  VulkanTriangle app;

  try {
    app.run();
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown error occurred." << std::endl;
  }

  std::cout << "Vulkan Triangle application finished successfully."
            << std::endl;
}

int main() { test1(); }
