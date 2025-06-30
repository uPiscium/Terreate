#pragma once

#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Exception {
class GraphicModuleError : public TerreateException {
public:
  GraphicModuleError(str const &message) : TerreateException(message) {}
};

class BufferError : public GraphicModuleError {
public:
  BufferError(str const &message) : GraphicModuleError(message) {}
};

class FontError : public GraphicModuleError {
public:
  FontError(str const &message) : GraphicModuleError(message) {}
};

class JobError : public GraphicModuleError {
public:
  JobError(str const &message) : GraphicModuleError(message) {}
};

class ScreenError : public GraphicModuleError {
public:
  ScreenError(str const &message) : GraphicModuleError(message) {}
};

class ShaderError : public GraphicModuleError {
public:
  ShaderError(str const &message) : GraphicModuleError(message) {}
};

class TextError : public GraphicModuleError {
public:
  TextError(str const &message) : GraphicModuleError(message) {}
};

class TextureError : public GraphicModuleError {
public:
  TextureError(str const &message) : GraphicModuleError(message) {}
};

class WindowError : public GraphicModuleError {
public:
  WindowError(str const &message) : GraphicModuleError(message) {}
};
} // namespace Terreate::Exception
