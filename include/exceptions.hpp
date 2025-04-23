#ifndef __TERREATE_EXCEPTIONS_HPP__
#define __TERREATE_EXCEPTIONS_HPP__

#include <types.hpp>

namespace Terreate::Exceptions {
using namespace Terreate::Types;

class TerreateException : public std::exception {
private:
  Str mMessage;

public:
  TerreateException(Str const &message) noexcept : mMessage(message) {}
  virtual ~TerreateException() noexcept {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
};

class ResourceException : public TerreateException {
public:
  ResourceException(Str const &message) noexcept : TerreateException(message) {}
};

/*
 * Core system exceptions
 */
class CoreException : public TerreateException {
public:
  CoreException(Str const &message) noexcept : TerreateException(message) {}
};

class ExecutorError : public CoreException {
public:
  ExecutorError(Str const &message) noexcept : CoreException(message) {}
};

class TaskError : public CoreException {
public:
  TaskError(Str const &message) noexcept : CoreException(message) {}
};

class NullReferenceException : public CoreException {
public:
  NullReferenceException(Str const &message) noexcept
      : CoreException(message) {}
};

class NullObjectException : public CoreException {
public:
  NullObjectException() : CoreException("Null object") {}
};

class NotImplementedException : public CoreException {
public:
  NotImplementedException() noexcept : CoreException("Not implemented") {}
  NotImplementedException(Str const &message) noexcept
      : CoreException(message) {}
};

/*
 * Audio system exceptions
 */
class AudioException : public TerreateException {
public:
  AudioException(Str const &message) noexcept : TerreateException(message) {}
};

class OpenALException : public AudioException {
public:
  OpenALException(Str const &message) noexcept : AudioException(message) {}
};

class SourceError : public AudioException {
public:
  SourceError(Str const &message) noexcept : AudioException(message) {}
};

/*
 * Graphics system exceptions
 */

class GraphicsException : public TerreateException {
private:
  Str mMessage;

public:
  GraphicsException(Str const &message) : TerreateException(message) {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
};

class BufferError : public GraphicsException {
public:
  BufferError(Str const &message) : GraphicsException(message) {}
};

class FontError : public GraphicsException {
public:
  FontError(Str const &message) : GraphicsException(message) {}
};

class JobError : public GraphicsException {
public:
  JobError(Str const &message) : GraphicsException(message) {}
};

class ScreenError : public GraphicsException {
public:
  ScreenError(Str const &message) : GraphicsException(message) {}
};

class ShaderError : public GraphicsException {
public:
  ShaderError(Str const &message) : GraphicsException(message) {}
};

class TextError : public GraphicsException {
public:
  TextError(Str const &message) : GraphicsException(message) {}
};

class TextureError : public GraphicsException {
public:
  TextureError(Str const &message) : GraphicsException(message) {}
};

class WindowError : public GraphicsException {
public:
  WindowError(Str const &message) : GraphicsException(message) {}
};

/*
 * IO system exceptions
 */
class IOException : public TerreateException {
public:
  IOException(Str const &message) noexcept : TerreateException(message) {}
};

class FileException : public IOException {
public:
  FileException(Str const &message) noexcept : IOException(message) {}
};
} // namespace Terreate::Exceptions

#endif // __TERREATE_EXCEPTIONS_HPP__
