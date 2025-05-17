#ifndef __TERREATE_GRAPHICS_SHADER_HPP__
#define __TERREATE_GRAPHICS_SHADER_HPP__

#include <core/math.hpp>
#include <core/object.hpp>
#include <core/type.hpp>
#include <graphic/GLdefs.hpp>

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

class Shader {
private:
  bool mCompiled = false;
  bool mLinked = false;
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
  unsigned getAttribute(Str const &name) const {
    return glGetAttribLocation(mShaderID, name.c_str());
  }
  /*
   * @brief: Getter for shader uniform ID.
   * @param: name: name of uniform
   * @return: uniform ID
   */
  unsigned getLocation(Str const &name) const {
    return glGetUniformLocation(mShaderID, name.c_str());
  }
  /*
   * @brief: Getter for uniform block index.
   * @param: name: name of uniform block
   * @return: uniform block index
   */
  unsigned getUniformBlockIndex(Str const &name) const {
    return glGetUniformBlockIndex(mShaderID, name.c_str());
  }
  /*
   * @brief: Getter for shader storage block index.
   * @param: name: name of storage block
   * @return: storage block index
   */
  unsigned getStorageBlockIndex(Str const &name) const {
    return glGetProgramResourceIndex(mShaderID, GL_SHADER_STORAGE_BLOCK,
                                     name.c_str());
  }
  /*
   * @brief: Getter for shader option.
   * @return: shader option
   */
  ShaderOption const &getOption() const { return mOption; }

  /*
   * @brief: Setter for shader option.
   * @param: option: shader option
   */
  void setUniform(ShaderOption const &option) { mOption = option; }
  /*
   * @brief: Setter for shader Bool uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, bool const &value) const {
    glUniform1i(this->getLocation(name), static_cast<i32>(value));
  }
  /*
   * @brief: Setter for shader Bool array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void setUniform(Str const &name, bool const *value, u32 const &count) const {
    glUniform1iv(this->getLocation(name), count,
                 reinterpret_cast<i32 const *>(value));
  }
  /*
   * @brief: Setter for shader Int uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, i32 const &value) const {
    glUniform1i(this->getLocation(name), value);
  }
  /*
   * @brief: Setter for shader Int array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void setUniform(Str const &name, i32 const *value, i32 const &count) const {
    glUniform1iv(this->getLocation(name), count, value);
  }
  /*
   * @brief: Setter for shader Int array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, Vec<i32> value) const {
    this->setUniform(name, value.data(), value.size());
  }
  /*
   * @brief: Setter for shader float uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, float const &value) const {
    glUniform1f(this->getLocation(name), value);
  }
  /*
   * @brief: Setter for shader float array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void setUniform(Str const &name, float const *value, u32 const &count) const {
    glUniform1fv(this->getLocation(name), count, value);
  }
  /*
   * @brief: Setter for shader float array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, Vec<float> value) const {
    this->setUniform(name, value.data(), value.size());
  }
  /*
   * @brief: Setter for shader vec2 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, vec2 const &value) const {
    glUniform2fv(this->getLocation(name), 1, &value[0]);
  }
  /*
   * @brief: Setter for shader vec3 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, vec3 const &value) const {
    glUniform3fv(this->getLocation(name), 1, &value[0]);
  }
  /*
   * @brief: Setter for shader vec4 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, vec4 const &value) const {
    glUniform4fv(this->getLocation(name), 1, &value[0]);
  }
  /*
   * @brief: Setter for shader mat2 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, mat2 const &value) const {
    glUniformMatrix2fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  }
  /*
   * @brief: Setter for shader mat3 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, mat3 const &value) const {
    glUniformMatrix3fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  }
  /*
   * @brief: Setter for shader mat4 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, mat4 const &value) const {
    glUniformMatrix4fv(this->getLocation(name), 1, GL_FALSE, &value[0][0]);
  }

  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: binding: binding point
   */
  void bindUniformBlock(Str const &name, u32 const &binding) const {
    glUniformBlockBinding(mShaderID, this->getUniformBlockIndex(name), binding);
  }
  /*
   * @brief: This function binds storage block index to binding point.
   * @param: name: name of storage block
   * @param: binding: binding point
   */
  void bindStorageBlock(Str const &name, u32 const &binding) const {
    glShaderStorageBlockBinding(mShaderID, this->getStorageBlockIndex(name),
                                binding);
  }
  /*
   * @brief: Compile shader with source code.
   */
  void compile(Str const &vertex, Str const &fragment,
               Str const &geometry = "");
  /*
   * @brief: Link shader.
   */
  void link();
  /*
   * @brief: Use shader.
   * @sa: Compile
   * @detail: This function should be called after shader is compiled.
   */
  void bind() const;
  /*
   * @brief: Unuse shader.
   */
  void unbind() const { glUseProgram(0); }

  operator bool() const { return mShaderID; }

public:
  /*
   * @brief: Load shader source from file.
   * @param: path: path to shader source file
   * @param: source: string to store source code
   * @return: source code
   */
  static Str loadShaderSource(Str const &path);
  /*
   * @brief: This function activates selected texture binding point.
   * @param: target: texture binding point
   */
  static void activateTexture(TextureTargets const &target) {
    glActiveTexture((u32)target);
  }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_SHADER_HPP__
