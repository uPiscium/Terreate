#pragma once

#include "type.hpp"

namespace Terreate::Exception {
class TerreateException : public std::exception {
private:
  str mMessage;

public:
  TerreateException(str const &message) noexcept : mMessage(message) {}
  virtual ~TerreateException() noexcept {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
};

class ResourceException : public TerreateException {
public:
  ResourceException(str const &message) noexcept : TerreateException(message) {}
};

class CoreException : public TerreateException {
public:
  CoreException(str const &message) noexcept : TerreateException(message) {}
};

class ExecutorError : public CoreException {
public:
  ExecutorError(str const &message) noexcept : CoreException(message) {}
};

class TaskError : public CoreException {
public:
  TaskError(str const &message) noexcept : CoreException(message) {}
};

class NullReferenceException : public CoreException {
public:
  NullReferenceException(str const &message) noexcept
      : CoreException(message) {}
};

class NullObjectException : public CoreException {
public:
  NullObjectException() : CoreException("Null object") {}
};

class NotImplementedException : public CoreException {
public:
  NotImplementedException() noexcept : CoreException("Not implemented") {}
  NotImplementedException(str const &message) noexcept
      : CoreException(message) {}
};

class AudioException : public TerreateException {
public:
  AudioException(str const &message) noexcept : TerreateException(message) {}
};

class OpenALException : public AudioException {
public:
  OpenALException(str const &message) noexcept : AudioException(message) {}
};

class SourceError : public AudioException {
public:
  SourceError(str const &message) noexcept : AudioException(message) {}
};

class GraphicsException : public TerreateException {
private:
  str mMessage;

public:
  GraphicsException(str const &message) : TerreateException(message) {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
};

class BufferError : public GraphicsException {
public:
  BufferError(str const &message) : GraphicsException(message) {}
};

class FontError : public GraphicsException {
public:
  FontError(str const &message) : GraphicsException(message) {}
};

class JobError : public GraphicsException {
public:
  JobError(str const &message) : GraphicsException(message) {}
};

class ScreenError : public GraphicsException {
public:
  ScreenError(str const &message) : GraphicsException(message) {}
};

class ShaderError : public GraphicsException {
public:
  ShaderError(str const &message) : GraphicsException(message) {}
};

class TextError : public GraphicsException {
public:
  TextError(str const &message) : GraphicsException(message) {}
};

class TextureError : public GraphicsException {
public:
  TextureError(str const &message) : GraphicsException(message) {}
};

class WindowError : public GraphicsException {
public:
  WindowError(str const &message) : GraphicsException(message) {}
};
} // namespace Terreate::Exception
