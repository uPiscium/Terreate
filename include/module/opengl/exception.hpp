#pragma once

#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Exception {
class OpenGLModuleError : public TerreateException {
public:
  OpenGLModuleError(str const &message) : TerreateException(message) {}
};

class BufferError : public OpenGLModuleError {
public:
  BufferError(str const &message) : OpenGLModuleError(message) {}
};

class FontError : public OpenGLModuleError {
public:
  FontError(str const &message) : OpenGLModuleError(message) {}
};

class ScreenError : public OpenGLModuleError {
public:
  ScreenError(str const &message) : OpenGLModuleError(message) {}
};

class ShaderError : public OpenGLModuleError {
public:
  ShaderError(str const &message) : OpenGLModuleError(message) {}
};

class TextError : public OpenGLModuleError {
public:
  TextError(str const &message) : OpenGLModuleError(message) {}
};

class TextureError : public OpenGLModuleError {
public:
  TextureError(str const &message) : OpenGLModuleError(message) {}
};
} // namespace Terreate::Exception
