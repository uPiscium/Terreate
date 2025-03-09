#ifndef __TERREATE_GRAPHICS_SHADER_HPP__
#define __TERREATE_GRAPHICS_SHADER_HPP__

#include <bindable.hpp>
#include <core/math.hpp>
#include <core/object.hpp>
#include <graphic/GLdefs.hpp>
#include <types.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;
using namespace Terreate::Math;

static Str const VERTEX_SHADER_SOURCE_DEFAULT = R"(
#version 460 core
void main() {
  gl_Position = vec4(position, 1.0);
}
)";

static Str const FRAGMENT_SHADER_SOURCE_DEFAULT = R"(
#version 460 core
out vec4 vColor;
void main() {
  vColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

class Shader : public Interface::IBindable {
private:
  Bool mCompiled = false;
  Bool mLinked = false;
  Core::Object mShaderID = Core::Object();
  ShaderOption mOption = ShaderOption();

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
   * @brief: Getter for shader option.
   * @return: shader option
   */
  ShaderOption const &GetOption() const { return mOption; }

  /*
   * @brief: Setter for shader option.
   * @param: option: shader option
   */
  void SetOption(ShaderOption const &option) { mOption = option; }
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
   * @brief: Compile shader with source code.
   */
  void Compile(Str const &vertex, Str const &fragment,
               Str const &geometry = "");
  /*
   * @brief: Link shader.
   */
  void Link();
  /*
   * @brief: Use shader.
   * @sa: Compile
   * @detail: This function should be called after shader is compiled.
   */
  void Bind() const;
  /*
   * @brief: Unuse shader.
   */
  void Unbind() const { glUseProgram(0); }

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
