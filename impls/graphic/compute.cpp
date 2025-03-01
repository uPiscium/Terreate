#include <exceptions.hpp>
#include <graphic/compute.hpp>

namespace Terreate::Graphic::Compute {
Str GetShaderLog(Uint const &id) {
  Int status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status) {
    return "";
  }

  char log[1024];
  glGetShaderInfoLog(id, 1024, NULL, log);
  return log;
}

void CheckCompileStatus(Uint const &id, Str const &name) {
  Str log = GetShaderLog(id);
  if (log != "") {
    log = name + " shader log: " + log;
    throw Exceptions::ShaderError(log);
  }
}

Str GetProgramLog(Uint const &id) {
  Int status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);

  if (status) {
    return "";
  }

  char log[1024];
  glGetProgramInfoLog(id, 1024, NULL, log);
  return log;
}

void CheckLinkStatus(Uint const &id) {
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
  if (mKernelID.Count() <= 1) {
    glDeleteProgram(mKernelID);
    mKernelID.Delete();
  }
}

void ComputeKernel::SetBool(Str const &name, Bool const &value) const {
  glUseProgram(mKernelID);
  glUniform1i(glGetUniformLocation(mKernelID, name.c_str()), value);
  glUseProgram(0);
}

void ComputeKernel::SetBools(Str const &name, Bool const *values,
                             Uint const &count) const {
  glUseProgram(mKernelID);
  glUniform1iv(glGetUniformLocation(mKernelID, name.c_str()), count,
               reinterpret_cast<Int const *>(values));
  glUseProgram(0);
}

void ComputeKernel::SetInt(Str const &name, Int const &value) const {
  glUseProgram(mKernelID);
  glUniform1i(glGetUniformLocation(mKernelID, name.c_str()), value);
  glUseProgram(0);
}

void ComputeKernel::SetInts(Str const &name, Int const *values,
                            Uint const &count) const {
  glUseProgram(mKernelID);
  glUniform1iv(glGetUniformLocation(mKernelID, name.c_str()), count, values);
  glUseProgram(0);
}

void ComputeKernel::SetInts(Str const &name, Vec<Int> values) const {
  glUseProgram(mKernelID);
  glUniform1iv(glGetUniformLocation(mKernelID, name.c_str()), values.size(),
               values.data());
  glUseProgram(0);
}

void ComputeKernel::SetFloat(Str const &name, Float const &value) const {
  glUseProgram(mKernelID);
  glUniform1f(glGetUniformLocation(mKernelID, name.c_str()), value);
  glUseProgram(0);
}

void ComputeKernel::SetFloats(Str const &name, Float const *values,
                              Uint const &count) const {
  glUseProgram(mKernelID);
  glUniform1fv(glGetUniformLocation(mKernelID, name.c_str()), count, values);
  glUseProgram(0);
}

void ComputeKernel::SetFloats(Str const &name, Vec<Float> values) const {
  glUseProgram(mKernelID);
  glUniform1fv(glGetUniformLocation(mKernelID, name.c_str()), values.size(),
               values.data());
  glUseProgram(0);
}

void ComputeKernel::SetVec2(Str const &name, vec2 const &value) const {
  glUseProgram(mKernelID);
  glUniform2f(glGetUniformLocation(mKernelID, name.c_str()), value.x, value.y);
  glUseProgram(0);
}

void ComputeKernel::SetVec3(Str const &name, vec3 const &value) const {
  glUseProgram(mKernelID);
  glUniform3f(glGetUniformLocation(mKernelID, name.c_str()), value.x, value.y,
              value.z);
  glUseProgram(0);
}

void ComputeKernel::SetVec4(Str const &name, vec4 const &value) const {
  glUseProgram(mKernelID);
  glUniform4f(glGetUniformLocation(mKernelID, name.c_str()), value.x, value.y,
              value.z, value.w);
  glUseProgram(0);
}

void ComputeKernel::SetMat2(Str const &name, mat2 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix2fv(glGetUniformLocation(mKernelID, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::SetMat3(Str const &name, mat3 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix3fv(glGetUniformLocation(mKernelID, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::SetMat4(Str const &name, mat4 const &value) const {
  glUseProgram(mKernelID);
  glUniformMatrix4fv(glGetUniformLocation(mKernelID, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
  glUseProgram(0);
}

void ComputeKernel::BindImage(Str const &name, Uint const &texture) const {
  this->SetInt(name, this->GetLocation(name));
  glBindImageTexture(this->GetLocation(name), texture, 0, GL_TRUE, 0,
                     GL_READ_WRITE, GL_RGBA32F);
}

void ComputeKernel::BindImage(Str const &name, Texture const &texture) const {
  this->SetInt(name, this->GetLocation(name));
  glBindImageTexture(this->GetLocation(name), texture.GetGLIndex(), 0, GL_TRUE,
                     0, GL_READ_WRITE, GL_RGBA32F);
}

void ComputeKernel::BindReadImage(Str const &name, Uint const &texture) const {
  this->SetInt(name, this->GetLocation(name));
  glBindImageTexture(this->GetLocation(name), texture, 0, GL_TRUE, 0,
                     GL_READ_ONLY, GL_RGBA32F);
}

void ComputeKernel::BindReadImage(Str const &name,
                                  Texture const &texture) const {
  this->SetInt(name, this->GetLocation(name));
  glBindImageTexture(this->GetLocation(name), texture.GetGLIndex(), 0, GL_TRUE,
                     0, GL_READ_ONLY, GL_RGBA32F);
}

void ComputeKernel::BindWriteImage(Str const &name, Uint const &texture) const {
  this->SetInt(name, this->GetLocation(name));
  glBindImageTexture(this->GetLocation(name), texture, 0, GL_TRUE, 0,
                     GL_WRITE_ONLY, GL_RGBA32F);
}

void ComputeKernel::BindWriteImage(Str const &name,
                                   Texture const &texture) const {
  this->SetInt(name, this->GetLocation(name));
  glBindImageTexture(this->GetLocation(name), texture.GetGLIndex(), 0, GL_TRUE,
                     0, GL_WRITE_ONLY, GL_RGBA32F);
}

void ComputeKernel::Compile() {
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

void ComputeKernel::Link() {
  if (!mCompiled) {
    throw Exceptions::ShaderError("Shader is not compiled");
    return;
  }

  glLinkProgram(mKernelID);
  CheckLinkStatus(mKernelID);
  mLinked = true;
}

void ComputeKernel::Dispatch(Uint const &x, Uint const &y,
                             Uint const &z) const {
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
