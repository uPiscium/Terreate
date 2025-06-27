#pragma once

#include "../common/enum.hpp"
#include "../common/math.hpp"
#include "../common/type.hpp"

#include "object.hpp"

namespace Terreate::OpenGL {
struct ShaderOption {
  bool blending = true;
  bool culling = false;
  bool depth = true;
  bool scissor = false;
  bool stencil = false;
  BlendingFunction src = BlendingFunction::SRC_ALPHA;
  BlendingFunction dst = BlendingFunction::ONE_MINUS_SRC_ALPHA;
  CullingFace cullFace = CullingFace::BACK;
  CullingMode frontFace = CullingMode::CCW;
  DepthFunction depthFunc = DepthFunction::LESS;
  StencilFunction stencilFunc = StencilFunction::ALWAYS;
  i32 stencilRef = 0;
  u32 stencilMask = 0xFF;
  StencilOperation sFail = StencilOperation::KEEP;
  StencilOperation dpFail = StencilOperation::KEEP;
  StencilOperation dpPass = StencilOperation::KEEP;
};

class Shader {
private:
  bool mCompiled = false;
  bool mLinked = false;
  Object mShaderID = Object();
  str mVertexShaderSource = "";
  str mFragmentShaderSource = "";
  str mGeometryShaderSource = "";
  ShaderOption mOption;

public:
  Shader();
  ~Shader();

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
  void setBlending(BlendingFunction const &src, BlendingFunction const &dst);
  void setCullingFace(CullingFace const &face,
                      CullingMode const &frontFace = CullingMode::CCW);
  void setDepth(DepthFunction const &func);
  void setStencilFunction(StencilFunction const &func, i32 const &ref,
                          u32 const &mask);
  void setStencilOperation(StencilOperation const &sFail,
                           StencilOperation const &dpFail,
                           StencilOperation const &dpPass);

  void addVertexShaderSource(str const &source) {
    mVertexShaderSource += source;
  }
  void addFragmentShaderSource(str const &source) {
    mFragmentShaderSource += source;
  }
  void addGeometryShaderSource(str const &source) {
    mGeometryShaderSource += source;
  }
  void bindUniformBlock(str const &name, u32 const &binding) const {
    glUniformBlockBinding(mShaderID, this->getUniformBlockIndex(name), binding);
  }
  void bindStorageBlock(str const &name, u32 const &binding) const {
    glShaderStorageBlockBinding(mShaderID, this->getStorageBlockIndex(name),
                                binding);
  }
  void useBlending(bool const &value) { mOption.blending = value; }
  void useCulling(bool const &value) { mOption.culling = value; }
  void useDepth(bool const &value) { mOption.depth = value; }
  void useScissor(bool const &value) { mOption.scissor = value; }
  void useStencil(bool const &value) { mOption.stencil = value; }
  void compile();
  void link();
  void use() const;
  void unuse() const { glUseProgram(0); }

  operator bool() const { return mShaderID; }

public:
  static str loadShaderSource(str const &path);
  static void activateTexture(TextureTargets const &target) {
    glActiveTexture((u32)target);
  }
};
} // namespace Terreate::OpenGL
