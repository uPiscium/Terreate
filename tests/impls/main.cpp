#include <core/context.hpp>
#include <core/debugger.hpp>
#include <core/window.hpp>

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

int main() {
  Core::Context ctx("Terreate", Type::Version(0, 1, 0));

  auto *debugger = new MyDebugger();
  ctx.attachDebugger(debugger);

  auto window = ctx.createWindow("Terreate", {1280, 720},
                                 Core::WindowSettings{.resizable = false});
  auto pipeline = ctx.createPipeline(window);
  auto framebuffer = ctx.createFramebuffer(pipeline);
  auto commandPool = ctx.createCommandPool(pipeline);
  auto commandBuffer = commandPool->createCommandBuffer();

  delete debugger;

  return 0;
}
