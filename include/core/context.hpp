#pragma once
#include "debugger.hpp"
#include "device.hpp"
#include "framebuffer.hpp"
#include "pipeline.hpp"
#include "window.hpp"

#include "../decl/type.hpp"
#include "../util/resourceptr.hpp"

namespace Terreate::Core {
class Context {
private:
  PROHIBIT_COPY_AND_ASSIGN(Context);

private:
  VkInstance mInstance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
  Util::ResourcePointerOwner<Device> mDevice = nullptr;

  Type::vec<Util::ResourcePointerOwner<Window>> mWindows;
  Type::vec<Util::ResourcePointerOwner<Pipeline>> mPipelines;
  Type::vec<Util::ResourcePointerOwner<Framebuffer>> mFramebuffers;

private:
  void loadEXTfunctions();
  Type::vec<char const *> getRequiredExts();
  bool checkValidationLayerSupport();
  VkDebugUtilsMessengerCreateInfoEXT createDebugInfo();
  VkApplicationInfo createAppInfo(char const *appName,
                                  Type::Version appVersion);

public:
  Context() = default;
  Context(Type::str const &appName, Type::Version appVersion);
  ~Context() { this->dispose(); }

  Util::ResourcePointer<Device> getDevice() const { return mDevice.get(); }

  void attachDebugger(IDebugger *debugger);

  Util::ResourcePointer<Window>
  createWindow(Type::str const &title, Type::pair<Type::i32> const &size,
               WindowSettings const &settings = WindowSettings());
  Util::ResourcePointer<Pipeline>
  createPipeline(Util::ResourcePointer<Window> window);
  Util::ResourcePointer<Framebuffer>
  createFramebuffer(Util::ResourcePointer<Pipeline> pipeline);

  void dispose();
};
} // namespace Terreate::Core
