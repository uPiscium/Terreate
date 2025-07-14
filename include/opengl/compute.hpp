#pragma once

#include "../common/enum.hpp"
#include "../common/math.hpp"
#include "../common/type.hpp"

#include "texture.hpp"

namespace Terreate::OpenGL {
str getShaderLog(u32 const &id);
void checkCompileStatus(u32 const &id, str const &name);
str getProgramLog(u32 const &id);
void checkLinkStatus(u32 const &id);

class ComputeKernel {
private:
  PROHIBIT_COPY_AND_ASSIGN(ComputeKernel);

private:
  bool mCompiled = false;
  bool mLinked = false;
  GLObject mKernelID = 0;
  str mKernelSource = "";

public:
  ComputeKernel();
  ~ComputeKernel();

  u32 getID() const { return mKernelID; }

  i32 getAttribute(str const &name) const {
    return glGetAttribLocation(mKernelID, name.c_str());
  }
  i32 getLocation(str const &name) const {
    return glGetUniformLocation(mKernelID, name.c_str());
  }
  u32 getStorageBlockIndex(str const &name) const {
    return glGetProgramResourceIndex(mKernelID, GL_SHADER_STORAGE_BLOCK,
                                     name.c_str());
  }

  void setUniform(str const &name, bool const &value) const;
  void setUniform(str const &name, bool const *value, u32 const &count) const;
  void setUniform(str const &name, i32 const &value) const;
  void setUniform(str const &name, i32 const *value, u32 const &count) const;
  void setUniform(str const &name, vec<i32> value) const;
  void setUniform(str const &name, float const &value) const;
  void setUniform(str const &name, float const *value, u32 const &count) const;
  void setUniform(str const &name, vec<float> value) const;
  void setUniform(str const &name, vec2 const &value) const;
  void setUniform(str const &name, vec3 const &value) const;
  void setUniform(str const &name, vec4 const &value) const;
  void setUniform(str const &name, mat2 const &value) const;
  void setUniform(str const &name, mat3 const &value) const;
  void setUniform(str const &name, mat4 const &value) const;

  void addKernelSource(str const &source) { mKernelSource += source; }

  void bindStorageBlock(str const &name, u32 const &binding) const {
    glShaderStorageBlockBinding(mKernelID, this->getStorageBlockIndex(name),
                                binding);
  }
  void bindImage(str const &name, u32 const &texture) const;
  void bindImage(str const &name, Texture const &texture) const;
  void bindReadImage(str const &name, u32 const &texture) const;
  void bindReadImage(str const &name, Texture const &texture) const;
  void bindWriteImage(str const &name, u32 const &texture) const;
  void bindWriteImage(str const &name, Texture const &texture) const;

  void compile();
  void link();
  void activateTexture(TextureTargets const &target) const;
  void dispatch(u32 const &x) const { this->dispatch(x, 1, 1); }
  void dispatch(u32 const &x, u32 const &y) const { this->dispatch(x, y, 1); }
  void dispatch(u32 const &x, u32 const &y, u32 const &z) const;

  operator bool() const { return mKernelID; }
};
} // namespace Terreate::OpenGL
