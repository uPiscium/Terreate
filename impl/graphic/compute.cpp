#include <core/exception.hpp>
#include <graphic/compute.hpp>

namespace Terreate::Graphic::Compute {
Str GetShaderLog(u32 const &id) {
  i32 status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status) {
    return "";
  }

  char log[1024];
  glGetShaderInfoLog(id, 1024, NULL, log);
  return log;
}

void CheckCompileStatus(u32 const &id, Str const &name) {
  Str log = GetShaderLog(id);
  if (log != "") {
    log = name + " shader log: " + log;
    throw Exceptions::ShaderError(log);
  }
}

Str GetProgramLog(u32 const &id) {
  i32 status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);

  if (status) {
    return "";
  }

  char log[1024];
  glGetProgramInfoLog(id, 1024, NULL, log);
  return log;
}

void CheckLinkStatus(u32 const &id) {
  Str log = GetProgramLog(id);

  if (log != "") {
    log = "Shader program log: " + log;
    throw Exceptions::ShaderError(log);
  }
}

ComputeKernel::ComputeKernel() {
  mKernelID = glCreateProgram();
  mCompiled = false;
}

ComputeKernel::~ComputeKernel() {
  if (mKernelID.count() <= 1) {
    glDeleteProgram(mKernelID);
    mKernelID.dispose();
  }
}

void ComputeKernel::setUniform(Str const &name, bool const &value) const {
  glUseProgram(mKernelID);
  glUniform1i(glGetUniformLocation(mKernelID, name.c_str()), value);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, bool const *values,
                               u32 const &count) const {
  glUseProgram(mKernelID);
  glUniform1iv(glGetUniformLocation(mKernelID, name.c_str()), count,
               reinterpret_cast<i32 const *>(values));
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, i32 const &value) const {
  glUseProgram(mKernelID);
  glUniform1i(glGetUniformLocation(mKernelID, name.c_str()), value);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, i32 const *values,
                               u32 const &count) const {
  glUseProgram(mKernelID);
  glUniform1iv(glGetUniformLocation(mKernelID, name.c_str()), count, values);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, Vec<i32> values) const {
  glUseProgram(mKernelID);
  glUniform1iv(glGetUniformLocation(mKernelID, name.c_str()), values.size(),
               values.data());
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, float const &value) const {
  glUseProgram(mKernelID);
  glUniform1f(glGetUniformLocation(mKernelID, name.c_str()), value);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, float const *values,
                               u32 const &count) const {
  glUseProgram(mKernelID);
  glUniform1fv(glGetUniformLocation(mKernelID, name.c_str()), count, values);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, Vec<float> values) const {
  glUseProgram(mKernelID);
  glUniform1fv(glGetUniformLocation(mKernelID, name.c_str()), values.size(),
               values.data());
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, vec2 const &value) const {
  glUseProgram(mKernelID);
  glUniform2f(glGetUniformLocation(mKernelID, name.c_str()), value.x, value.y);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, vec3 const &value) const {
  glUseProgram(mKernelID);
  glUniform3f(glGetUniformLocation(mKernelID, name.c_str()), value.x, value.y,
              value.z);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, vec4 const &value) const {
  glUseProgram(mKernelID);
  glUniform4f(glGetUniformLocation(mKernelID, name.c_str()), value.x, value.y,
              value.z, value.w);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, mat2 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix2fv(glGetUniformLocation(mKernelID, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, mat3 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix3fv(glGetUniformLocation(mKernelID, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::setUniform(Str const &name, mat4 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix4fv(glGetUniformLocation(mKernelID, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::bindImage(Str const &name, u32 const &texture) const {
  this->setUniform(name, (i32)this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture, 0, GL_TRUE, 0,
                     GL_READ_WRITE, GL_RGBA32F);
}

void ComputeKernel::bindImage(Str const &name, Texture const &texture) const {
  this->setUniform(name, (i32)this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture.getGLIndex(), 0, GL_TRUE,
                     0, GL_READ_WRITE, GL_RGBA32F);
}

void ComputeKernel::bindReadImage(Str const &name, u32 const &texture) const {
  this->setUniform(name, (i32)this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture, 0, GL_TRUE, 0,
                     GL_READ_ONLY, GL_RGBA32F);
}

void ComputeKernel::bindReadImage(Str const &name,
                                  Texture const &texture) const {
  this->setUniform(name, (i32)this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture.getGLIndex(), 0, GL_TRUE,
                     0, GL_READ_ONLY, GL_RGBA32F);
}

void ComputeKernel::bindWriteImage(Str const &name, u32 const &texture) const {
  this->setUniform(name, (i32)this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture, 0, GL_TRUE, 0,
                     GL_WRITE_ONLY, GL_RGBA32F);
}

void ComputeKernel::bindWriteImage(Str const &name,
                                   Texture const &texture) const {
  this->setUniform(name, (i32)this->getLocation(name));
  glBindImageTexture(this->getLocation(name), texture.getGLIndex(), 0, GL_TRUE,
                     0, GL_WRITE_ONLY, GL_RGBA32F);
}

void ComputeKernel::compile() {
  if (mKernelSource == "") {
    throw Exceptions::ShaderError("Compute kernel source is empty");
    return;
  }

  ID kernelID = 0;
  kernelID = glCreateShader(GL_COMPUTE_SHADER);
  char const *kernelSource = mKernelSource.c_str();
  glShaderSource(kernelID, 1, &kernelSource, nullptr);
  glCompileShader(kernelID);
  CheckCompileStatus(kernelID, "Compute");

  glAttachShader(mKernelID, kernelID);
  glDeleteShader(kernelID);

  mCompiled = true;
}

void ComputeKernel::link() {
  if (!mCompiled) {
    throw Exceptions::ShaderError("Shader is not compiled");
    return;
  }

  glLinkProgram(mKernelID);
  CheckLinkStatus(mKernelID);
  mLinked = true;
}

void ComputeKernel::dispatch(u32 const &x, u32 const &y, u32 const &z) const {
  if (!mLinked) {
    throw Exceptions::ShaderError("Kernel is not linked");
    return;
  }

  glUseProgram(mKernelID);
  glDispatchCompute(x, y, z);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT |
                  GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  glUseProgram(0);
}
} // namespace Terreate::Graphic::Compute
