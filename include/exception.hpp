#pragma once
#include <exception>
#include <type.hpp>

namespace Terreate::Exception {
using namespace Terreate::Type;

class TerreateException : public std::exception {
private:
  str mMessage;

public:
  TerreateException(str const &message) : mMessage(message) {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
  virtual str message() const noexcept { return mMessage; }
};

class NullReferenceException : public TerreateException {
public:
  NullReferenceException(str const &message) : TerreateException(message) {}
};

class NotImplementedException : public TerreateException {
public:
  NotImplementedException()
      : TerreateException("This function is not implemented.") {}
};

class GLFWInitializeFailure : public TerreateException {
public:
  GLFWInitializeFailure()
      : TerreateException("Failed to initialize GLFW library.") {}
};

class VulkanAPIError : public TerreateException {
public:
  VulkanAPIError(str const &message) : TerreateException(message) {}
};

class ValidationLayerNotSupported : public VulkanAPIError {
public:
  ValidationLayerNotSupported()
      : VulkanAPIError("Validation layer is not supported.") {}
};

class DebugMessengerFunctionLoadFailure : public VulkanAPIError {
public:
  DebugMessengerFunctionLoadFailure()
      : VulkanAPIError("Failed to load debug messenger functions.") {}
};

class DebugMessengerCreationFailure : public VulkanAPIError {
public:
  DebugMessengerCreationFailure()
      : VulkanAPIError("Failed to create debug messenger.") {}
};

class InstanceCreationFailure : public VulkanAPIError {
public:
  InstanceCreationFailure()
      : VulkanAPIError("Failed to create Vulkan instance.") {}
};

class NoDeviceFound : public VulkanAPIError {
public:
  NoDeviceFound()
      : VulkanAPIError("Failed to find a suitable GPU with Vulkan support.") {}
};

class ImageViewCreationFailure : public VulkanAPIError {
public:
  ImageViewCreationFailure() : VulkanAPIError("Failed to create image view.") {}
};

class SwapchainCreationFailure : public VulkanAPIError {
public:
  SwapchainCreationFailure()
      : VulkanAPIError("Failed to create Vulkan swapchain.") {}
};

class ShaderModuleCreationFailure : public VulkanAPIError {
public:
  ShaderModuleCreationFailure()
      : VulkanAPIError("Failed to create shader module.") {}
};

class RenderPassCreationFailure : public VulkanAPIError {
public:
  RenderPassCreationFailure()
      : VulkanAPIError("Failed to create render pass.") {}
};

class PipelineCreationFailure : public VulkanAPIError {
public:
  PipelineCreationFailure()
      : VulkanAPIError("Failed to create graphics pipeline.") {}
};

class GraphicException : public TerreateException {
public:
  GraphicException(str const &message) : TerreateException(message) {}
};

class WindowCreationFailure : public GraphicException {
public:
  WindowCreationFailure() : GraphicException("Failed to create window.") {}
};

class SurfaceCreationFailure : public GraphicException {
public:
  SurfaceCreationFailure()
      : GraphicException("Failed to create Vulkan surface.") {}
};

} // namespace Terreate::Exception
