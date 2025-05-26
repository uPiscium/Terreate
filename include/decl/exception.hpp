#pragma once
#include "type.hpp"

#include <exception>

namespace Terreate::Exception {
class TerreateException : public std::exception {
private:
  Type::str mMessage;

public:
  TerreateException(Type::str const &message) : mMessage(message) {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
  virtual Type::str message() const noexcept { return mMessage; }
};

class NullReferenceException : public TerreateException {
public:
  NullReferenceException(Type::str const &message)
      : TerreateException(message) {}
};

class NotImplementedException : public TerreateException {
public:
  NotImplementedException()
      : TerreateException("This function is not implemented.") {}
};

class GLFWInitializeFailure : public TerreateException {
public:
  GLFWInitializeFailure(Type::str const &message)
      : TerreateException(message) {}
};

class VulkanAPIError : public TerreateException {
public:
  VulkanAPIError(Type::str const &message) : TerreateException(message) {}
};

class ValidationLayerNotSupported : public VulkanAPIError {
public:
  ValidationLayerNotSupported(Type::str const &message)
      : VulkanAPIError(message) {}
};

class DebugMessengerFunctionLoadFailure : public VulkanAPIError {
public:
  DebugMessengerFunctionLoadFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class DebugMessengerCreationFailure : public VulkanAPIError {
public:
  DebugMessengerCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class InstanceCreationFailure : public VulkanAPIError {
public:
  InstanceCreationFailure(Type::str const &message) : VulkanAPIError(message) {}
};

class NoQueueFamilyFound : public VulkanAPIError {
public:
  NoQueueFamilyFound(Type::str const &message) : VulkanAPIError(message) {}
};

class NoDeviceFound : public VulkanAPIError {
public:
  NoDeviceFound(Type::str const &message) : VulkanAPIError(message) {}
};

class DeviceCreationFailure : public VulkanAPIError {
public:
  DeviceCreationFailure(Type::str const &message) : VulkanAPIError(message) {}
};

class ImageViewCreationFailure : public VulkanAPIError {
public:
  ImageViewCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class SwapchainCreationFailure : public VulkanAPIError {
public:
  SwapchainCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class ShaderModuleCreationFailure : public VulkanAPIError {
public:
  ShaderModuleCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class RenderPassCreationFailure : public VulkanAPIError {
public:
  RenderPassCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class PipelineLayoutCreationFailure : public VulkanAPIError {
public:
  PipelineLayoutCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class PipelineCreationFailure : public VulkanAPIError {
public:
  PipelineCreationFailure(Type::str const &message) : VulkanAPIError(message) {}
};

class FramebufferCreationFailure : public VulkanAPIError {
public:
  FramebufferCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class CommandPoolCreationFailure : public VulkanAPIError {
public:
  CommandPoolCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class CommandBufferCreationFailure : public VulkanAPIError {
public:
  CommandBufferCreationFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class CommandBufferRecordingFailure : public VulkanAPIError {
public:
  CommandBufferRecordingFailure(Type::str const &message)
      : VulkanAPIError(message) {}
};

class GraphicException : public TerreateException {
public:
  GraphicException(Type::str const &message) : TerreateException(message) {}
};

class WindowCreationFailure : public GraphicException {
public:
  WindowCreationFailure(Type::str const &message) : GraphicException(message) {}
};

class SurfaceCreationFailure : public GraphicException {
public:
  SurfaceCreationFailure(Type::str const &message)
      : GraphicException(message) {}
};

} // namespace Terreate::Exception
