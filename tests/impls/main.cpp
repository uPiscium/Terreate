#include "../include/main.hpp"
#include <iostream>

using namespace Terreate;
using namespace Terreate::Type;

class MyDebugger : public Util::IDebugger {
public:
  virtual bool verbose(str const &message, Util::MessageType const type,
                       vec<Util::DebugObject> const &object) override {
    // std::cout << "Verbose: " << message << std::endl;
    return false;
  }

  virtual bool info(str const &message, Util::MessageType const type,
                    vec<Util::DebugObject> const &object) override {
    // std::cout << "Info: " << message << std::endl;
    return false;
  }

  virtual bool warning(str const &message, Util::MessageType const type,
                       vec<Util::DebugObject> const &object) override {
    std::cout << "Warning: " << message << std::endl;
    return false;
  }

  virtual bool error(str const &message, Util::MessageType const type,
                     vec<Util::DebugObject> const &object) override {
    std::cout << "Error: " << message << std::endl;
    return false;
  }
};

int main() {
  Core::Context context;
  Core::Instance instance = context.createInstance(
      "Terreate", Core::Version(1, 0, 0), "Terreate Engine",
      Core::Version(1, 0, 0), VK_API_VERSION_1_4);

  auto *debugger = new MyDebugger();
  instance.attachDebugger(debugger);
  delete debugger;

  return 0;
}
