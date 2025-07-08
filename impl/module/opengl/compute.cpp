#include "../../../include/module/opengl/compute.hpp"
#include "../../../include/module/opengl/exception.hpp"

namespace Terreate::OpenGL {
str getShaderLog(u32 const &id) {
  i32 status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status) {
    return "";
  }

  char log[1024];
  glGetShaderInfoLog(id, 1024, NULL, log);
  return log;
}

void checkCompileStatus(u32 const &id, str const &name) {
  str log = getShaderLog(id);
  if (log != "") {
    log = name + " shader log: " + log;
    throw Exception::ShaderError(log);
  }
}

str getProgramLog(u32 const &id) {
  i32 status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);

  if (status) {
    return "";
  }

  char log[1024];
  glGetProgramInfoLog(id, 1024, NULL, log);
  return log;
}

void checkLinkStatus(u32 const &id) {
  str log = getProgramLog(id);

  if (log != "") {
    log = "Shader program log: " + log;
    throw Exception::ShaderError(log);
  }
}

ComputeKernel::ComputeKernel() {
  mKernelID = glCreateProgram();
  mCompiled = false;
}

ComputeKernel::~ComputeKernel() {
  if (mKernelID != 0) {
    glDeleteProgram(mKernelID);
    mKernelID = 0;
  }
}

void ComputeKernel::setUniform(str const &name, bool const &value) const {
  glUseProgram(mKernelID);
  glUniform1i(this->getLocation(name), value);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, bool const *values,
                               u32 const &count) const {
  glUseProgram(mKernelID);
  glUniform1iv(this->getLocation(name), count,
               reinterpret_cast<i32 const *>(values));
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, i32 const &value) const {
  glUseProgram(mKernelID);
  glUniform1i(this->getLocation(name), value);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, i32 const *values,
                               u32 const &count) const {
  glUseProgram(mKernelID);
  glUniform1iv(this->getLocation(name), count, values);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, vec<i32> values) const {
  glUseProgram(mKernelID);
  glUniform1iv(this->getLocation(name), values.size(), values.data());
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, float const &value) const {
  glUseProgram(mKernelID);
  glUniform1f(this->getLocation(name), value);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, float const *values,
                               u32 const &count) const {
  glUseProgram(mKernelID);
  glUniform1fv(this->getLocation(name), count, values);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, vec<float> values) const {
  glUseProgram(mKernelID);
  glUniform1fv(this->getLocation(name), values.size(), values.data());
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, vec2 const &value) const {
  glUseProgram(mKernelID);
  glUniform2f(this->getLocation(name), value.x, value.y);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, vec3 const &value) const {
  glUseProgram(mKernelID);
  glUniform3f(this->getLocation(name), value.x, value.y, value.z);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, vec4 const &value) const {
  glUseProgram(mKernelID);
  glUniform4f(this->getLocation(name), value.x, value.y, value.z, value.w);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, mat2 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix2fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, mat3 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix3fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::setUniform(str const &name, mat4 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix4fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::bindImage(str const &name, u32 const &texture) const {
  this->setUniform(name, this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture, 0, GL_TRUE, 0,
                     GL_READ_WRITE, GL_RGBA32F);
}

void ComputeKernel::bindImage(str const &name, Texture const &texture) const {
  this->setUniform(name, this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture.getGLIndex(), 0, GL_TRUE,
                     0, GL_READ_WRITE, GL_RGBA32F);
}

void ComputeKernel::bindReadImage(str const &name, u32 const &texture) const {
  this->setUniform(name, this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture, 0, GL_TRUE, 0,
                     GL_READ_ONLY, GL_RGBA32F);
}

void ComputeKernel::bindReadImage(str const &name,
                                  Texture const &texture) const {
  this->setUniform(name, this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture.getGLIndex(), 0, GL_TRUE,
                     0, GL_READ_ONLY, GL_RGBA32F);
}

void ComputeKernel::bindWriteImage(str const &name, u32 const &texture) const {
  this->setUniform(name, this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture, 0, GL_TRUE, 0,
                     GL_WRITE_ONLY, GL_RGBA32F);
}

void ComputeKernel::bindWriteImage(str const &name,
                                   Texture const &texture) const {
  this->setUniform(name, this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture.getGLIndex(), 0, GL_TRUE,
                     0, GL_WRITE_ONLY, GL_RGBA32F);
}

void ComputeKernel::compile() {
  if (mKernelSource == "") {
    throw Exception::ShaderError("Compute kernel source is empty");
    return;
  }

  u32 kernelID = 0;
  kernelID = glCreateShader(GL_COMPUTE_SHADER);
  char const *kernelSource = mKernelSource.c_str();
  glShaderSource(kernelID, 1, &kernelSource, nullptr);
  glCompileShader(kernelID);
  checkCompileStatus(kernelID, "Compute");

  glAttachShader(mKernelID, kernelID);
  glDeleteShader(kernelID);

  mCompiled = true;
}

void ComputeKernel::link() {
  if (!mCompiled) {
    throw Exception::ShaderError("Shader is not compiled");
    return;
  }

  glLinkProgram(mKernelID);
  checkLinkStatus(mKernelID);
  mLinked = true;
}

void ComputeKernel::dispatch(u32 const &x, u32 const &y, u32 const &z) const {
  if (!mLinked) {
    throw Exception::ShaderError("Kernel is not linked");
    return;
  }

  glUseProgram(mKernelID);
  glDispatchCompute(x, y, z);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT |
                  GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  glUseProgram(0);
}
} // namespace Terreate::OpenGL
