#include <core/instance.hpp>
#include <exception.hpp>

namespace Terreate::Core {
Instance::Instance() : mInstance(nullptr) {}

Instance::Instance(Instance const &instance) : mInstance(instance.mInstance) {}

Instance::Instance(VkInstance const instance) : mInstance(instance) {}

Instance::~Instance() {}

void Instance::attachDebugger(Util::IDebugger *debugger) {
#ifdef TERREATE_DEBUG_BUILD
  if (!debugger)
    return;

  if (API::checkValidationLayerSupport()) {
    auto debuggerInfo = API::createDebugCreateInfo();
    debuggerInfo.pUserData = debugger;
    debuggerInfo.pNext = nullptr;
    if (API::trCreateDebugUtilsMessengerEXT(mInstance, &debuggerInfo, nullptr,
                                            &mDebugMessenger) != VK_SUCCESS) {
      throw Exception::DebugMessengerCreationFailure();
    }
  } else {
    throw Exception::ValidationLayerNotSupported();
  }
#endif
}
} // namespace Terreate::Core
