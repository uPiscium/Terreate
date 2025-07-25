#pragma once

#include "../common/enum.hpp"
#include "../common/math.hpp"
#include "../common/type.hpp"

namespace Terreate::OpenGL {
struct Blending {
  bool enable = true;
  BlendingFunction src = BlendingFunction::SRC_ALPHA;
  BlendingFunction dst = BlendingFunction::ONE_MINUS_SRC_ALPHA;

  void apply() const;
};

struct Culling {
  bool enable = false;
  CullingFace face = CullingFace::BACK;
  CullingMode frontFace = CullingMode::CCW;

  void apply() const;
};

struct Depth {
  bool enable = true;
  bool writable = true;
  float near = 0.0f;
  float far = 1.0f;
  float offsetFactor = 0.0f;
  float offsetUnits = 0.0f;
  DepthFunction func = DepthFunction::LESS;

  void apply() const;
};

struct Scissor {
  bool enable = false;
  i32 x = 0;
  i32 y = 0;
  u32 width = 0;
  u32 height = 0;

  void apply() const;
};

struct Stencil {
  bool enable = false;
  StencilFunction func = StencilFunction::ALWAYS;
  i32 ref = 0;
  u32 mask = 0xFF;
  StencilOperation sFail = StencilOperation::KEEP;
  StencilOperation dpFail = StencilOperation::KEEP;
  StencilOperation dpPass = StencilOperation::KEEP;

  void apply() const;
};

class ShaderProperty {
public:
  Blending blending;
  Culling culling;
  Depth depth;
  Scissor scissor;
  Stencil stencil;
};

class Shader {
private:
  PROHIBIT_COPY_AND_ASSIGN(Shader);

private:
  bool mCompiled = false;
  bool mLinked = false;
  GLObject mShaderID = 0;
  str mVertexShaderSource = "";
  str mFragmentShaderSource = "";
  ShaderProperty mProperties;

public:
  Shader();
  ~Shader();

  ShaderProperty const &getProperties() const { return mProperties; }
  ShaderProperty &getProperties() { return mProperties; }
  u32 getAttribute(str const &name) const {
    return glGetAttribLocation(mShaderID, name.c_str());
  }
  u32 getLocation(str const &name) const {
    return glGetUniformLocation(mShaderID, name.c_str());
  }
  u32 getUniformBlockIndex(str const &name) const {
    return glGetUniformBlockIndex(mShaderID, name.c_str());
  }
  u32 getStorageBlockIndex(str const &name) const {
    return glGetProgramResourceIndex(mShaderID, GL_SHADER_STORAGE_BLOCK,
                                     name.c_str());
  }

  void setUniform(str const &name, bool const &value) const {
    glUniform1i(this->getLocation(name), static_cast<i32>(value));
  }
  void setUniform(str const &name, bool const *value, u32 const &count) const {
    glUniform1iv(this->getLocation(name), count,
                 reinterpret_cast<i32 const *>(value));
  }
  void setUniform(str const &name, i32 const &value) const {
    glUniform1i(this->getLocation(name), value);
  }
  void setUniform(str const &name, i32 const *value, u32 const &count) const {
    glUniform1iv(this->getLocation(name), count, value);
  }
  void setUniform(str const &name, vec<i32> value) const {
    this->setUniform(name, value.data(), value.size());
  }
  void setUniform(str const &name, float const &value) const {
    glUniform1f(this->getLocation(name), value);
  }
  void setUniform(str const &name, float const *value, u32 const &count) const {
    glUniform1fv(this->getLocation(name), count, value);
  }
  void setUniform(str const &name, vec<float> value) const {
    this->setUniform(name, value.data(), value.size());
  }
  void setUniform(str const &name, vec2 const &value) const {
    glUniform2fv(this->getLocation(name), 1, &value[0]);
  }
  void setUniform(str const &name, vec3 const &value) const {
    glUniform3fv(this->getLocation(name), 1, &value[0]);
  }
  void setUniform(str const &name, vec4 const &value) const {
    glUniform4fv(this->getLocation(name), 1, &value[0]);
  }
  void setUniform(str const &name, mat2 const &value) const {
    glUniformMatrix2fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  }
  void setUniform(str const &name, mat3 const &value) const {
    glUniformMatrix3fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  }
  void setUniform(str const &name, mat4 const &value) const {
    glUniformMatrix4fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  }

  void addVertexShaderSource(str const &source) {
    mVertexShaderSource += source;
  }
  void addFragmentShaderSource(str const &source) {
    mFragmentShaderSource += source;
  }
  void bindUniformBlock(str const &name, u32 const &binding) const {
    glUniformBlockBinding(mShaderID, this->getUniformBlockIndex(name), binding);
  }
  void bindStorageBlock(str const &name, u32 const &binding) const {
    glShaderStorageBlockBinding(mShaderID, this->getStorageBlockIndex(name),
                                binding);
  }
  void compile();
  void link();
  void bind() const;
  void unbind() const { glUseProgram(0); }

  operator bool() const { return mShaderID; }

public:
  static str loadShaderSource(str const &path);
  static void activateTexture(TextureTargets const &target) {
    glActiveTexture((u32)target);
  }
};
} // namespace Terreate::OpenGL
