#ifndef __TERREATE_GRAPHICS_SHADER_HPP__
#define __TERREATE_GRAPHICS_SHADER_HPP__

#include <core/math.hpp>
#include <core/object.hpp>
#include <graphic/GLdefs.hpp>
#include <types.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;
using namespace Terreate::Math;

struct ShaderOption {
  Bool blending = true;
  Bool culling = false;
  Bool depth = true;
  Bool scissor = false;
  Bool stencil = false;
  BlendingFunction src = BlendingFunction::SRC_ALPHA;
  BlendingFunction dst = BlendingFunction::ONE_MINUS_SRC_ALPHA;
  CullingFace cullFace = CullingFace::BACK;
  CullingMode frontFace = CullingMode::CCW;
  DepthFunction depthFunc = DepthFunction::LESS;
  StencilFunction stencilFunc = StencilFunction::ALWAYS;
  Int stencilRef = 0;
  Uint stencilMask = 0xFF;
  StencilOperation sFail = StencilOperation::KEEP;
  StencilOperation dpFail = StencilOperation::KEEP;
  StencilOperation dpPass = StencilOperation::KEEP;
};

class Shader final {
private:
  Bool mCompiled = false;
  Bool mLinked = false;
  Core::Object mShaderID = Core::Object();
  Str mVertexShaderSource = "";
  Str mFragmentShaderSource = "";
  Str mGeometryShaderSource = "";
  ShaderOption mOption;

public:
  /*
   * @brief: OpenGL shader wrapper class. This class handles vertex, fragment,
   * and geometry shaders.
   */
  Shader();
  ~Shader();

  /*
   * @brief: Get attribute index
   * @param: name: name of attribute
   * @return: attribute index
   */
  unsigned GetAttribute(Str const &name) const {
    return glGetAttribLocation(mShaderID, name.c_str());
  }
  /*
   * @brief: Getter for shader uniform ID.
   * @param: name: name of uniform
   * @return: uniform ID
   */
  unsigned GetLocation(Str const &name) const {
    return glGetUniformLocation(mShaderID, name.c_str());
  }
  /*
   * @brief: Getter for uniform block index.
   * @param: name: name of uniform block
   * @return: uniform block index
   */
  unsigned GetUniformBlockIndex(Str const &name) const {
    return glGetUniformBlockIndex(mShaderID, name.c_str());
  }
  /*
   * @brief: Getter for shader storage block index.
   * @param: name: name of storage block
   * @return: storage block index
   */
  unsigned GetStorageBlockIndex(Str const &name) const {
    return glGetProgramResourceIndex(mShaderID, GL_SHADER_STORAGE_BLOCK,
                                     name.c_str());
  }

  /*
   * @brief: Setter for shader Bool uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetBool(Str const &name, Bool const &value) const {
    glUniform1i(this->GetLocation(name), static_cast<Int>(value));
  }
  /*
   * @brief: Setter for shader Bool array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void SetBools(Str const &name, Bool const *value, Uint const &count) const {
    glUniform1iv(this->GetLocation(name), count,
                 reinterpret_cast<Int const *>(value));
  }
  /*
   * @brief: Setter for shader Int uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetInt(Str const &name, Int const &value) const {
    glUniform1i(this->GetLocation(name), value);
  }
  /*
   * @brief: Setter for shader Int array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void SetInts(Str const &name, Int const *value, Uint const &count) const {
    glUniform1iv(this->GetLocation(name), count, value);
  }
  /*
   * @brief: Setter for shader Int array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetInts(Str const &name, Vec<Int> value) const {
    this->SetInts(name, value.data(), value.size());
  }
  /*
   * @brief: Setter for shader float uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetFloat(Str const &name, Float const &value) const {
    glUniform1f(this->GetLocation(name), value);
  }
  /*
   * @brief: Setter for shader float array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void SetFloats(Str const &name, Float const *value, Uint const &count) const {
    glUniform1fv(this->GetLocation(name), count, value);
  }
  /*
   * @brief: Setter for shader float array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetFloats(Str const &name, Vec<Float> value) const {
    this->SetFloats(name, value.data(), value.size());
  }
  /*
   * @brief: Setter for shader vec2 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetVec2(Str const &name, vec2 const &value) const {
    glUniform2fv(this->GetLocation(name), 1, &value[0]);
  }
  /*
   * @brief: Setter for shader vec3 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetVec3(Str const &name, vec3 const &value) const {
    glUniform3fv(this->GetLocation(name), 1, &value[0]);
  }
  /*
   * @brief: Setter for shader vec4 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetVec4(Str const &name, vec4 const &value) const {
    glUniform4fv(this->GetLocation(name), 1, &value[0]);
  }
  /*
   * @brief: Setter for shader mat2 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetMat2(Str const &name, mat2 const &value) const {
    glUniformMatrix2fv(this->GetLocation(name), 1, GL_FALSE, &value[0][0]);
  }
  /*
   * @brief: Setter for shader mat3 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetMat3(Str const &name, mat3 const &value) const {
    glUniformMatrix3fv(this->GetLocation(name), 1, GL_FALSE, &value[0][0]);
  }
  /*
   * @brief: Setter for shader mat4 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void SetMat4(Str const &name, mat4 const &value) const {
    glUniformMatrix4fv(this->GetLocation(name), 1, GL_FALSE, &value[0][0]);
  }
  /*
   * @brief: Setter for blending function.
   * @param: src: source blending function
   * @param: dst: destination blending function
   */
  void SetBlending(BlendingFunction const &src, BlendingFunction const &dst);
  /*
   * @brief: Setter for culling face.
   * @param: face: face to cull
   */
  void SetCullingFace(CullingFace const &face,
                      CullingMode const &frontFace = CullingMode::CCW);
  /*
   * @brief: Setter for depth function.
   * @param: func: depth function
   */
  void SetDepth(DepthFunction const &func);
  /*
   * @brief: Setter for stencil function.
   * @param: func: stencil function
   */
  void SetStencilFunction(StencilFunction const &func, Int const &ref,
                          Uint const &mask);
  /*
   * @brief: Setter for stencil operation.
   * @param: sFail: stencil fail operation
   * @param: dpFail: stencil depth fail operation
   * @param: dpPass: stencil depth pass operation
   */
  void SetStencilOperation(StencilOperation const &sFail,
                           StencilOperation const &dpFail,
                           StencilOperation const &dpPass);

  /*
   * @brief: Add vertex shader source to current source.
   * @param: source: source code to add
   */
  void AddVertexShaderSource(Str const &source) {
    mVertexShaderSource += source;
  }
  /*
   * @brief: Add fragment shader source to current source.
   * @param: source: source code to add
   */
  void AddFragmentShaderSource(Str const &source) {
    mFragmentShaderSource += source;
  }
  /*
   * @brief: Add geometry shader source to current source.
   * @param: source: source code to add
   */
  void AddGeometryShaderSource(Str const &source) {
    mGeometryShaderSource += source;
  }
  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: binding: binding point
   */
  void BindUniformBlock(Str const &name, Uint const &binding) const {
    glUniformBlockBinding(mShaderID, this->GetUniformBlockIndex(name), binding);
  }
  /*
   * @brief: This function binds storage block index to binding point.
   * @param: name: name of storage block
   * @param: binding: binding point
   */
  void BindStorageBlock(Str const &name, Uint const &binding) const {
    glShaderStorageBlockBinding(mShaderID, this->GetStorageBlockIndex(name),
                                binding);
  }
  /*
   * @brief: This function swiches blending on or off.
   * @param: value: true to turn on, false to turn off
   */
  void UseBlending(Bool const &value) { mOption.blending = value; }
  /*
   * @brief: This function swiches culling on or off.
   * @param: value: true to turn on, false to turn off
   */
  void UseCulling(Bool const &value) { mOption.culling = value; }
  /*
   * @brief: This function swiches depth testing on or off.
   * @param: value: true to turn on, false to turn off
   */
  void UseDepth(Bool const &value) { mOption.depth = value; }
  /*
   * @brief: This function swiches scissor testing on or off.
   * @param: value: true to turn on, false to turn off
   */
  void UseScissor(Bool const &value) { mOption.scissor = value; }
  /*
   * @brief: This function swiches stencil testing on or off.
   * @param: value: true to turn on, false to turn off
   */
  void UseStencil(Bool const &value) { mOption.stencil = value; }
  /*
   * @brief: Compile shader.
   */
  void Compile();
  /*
   * @brief: Link shader.
   */
  void Link();
  /*
   * @brief: Use shader.
   * @sa: Compile
   * @detail: This function should be called after shader is compiled.
   */
  void Use() const;
  /*
   * @brief: Unuse shader.
   */
  void Unuse() const { glUseProgram(0); }

  operator Bool() const { return mShaderID; }

public:
  /*
   * @brief: Load shader source from file.
   * @param: path: path to shader source file
   * @param: source: string to store source code
   * @return: source code
   */
  static Str LoadShaderSource(Str const &path);
  /*
   * @brief: This function activates selected texture binding point.
   * @param: target: texture binding point
   */
  static void ActivateTexture(TextureTargets const &target) {
    glActiveTexture((Uint)target);
  }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_SHADER_HPP__
