#ifndef __TERREATE_GRAPHICS_KERNEL_HPP__
#define __TERREATE_GRAPHICS_KERNEL_HPP__

#include <core/math.hpp>
#include <core/object.hpp>
#include <core/type.hpp>
#include <graphic/GLdefs.hpp>
#include <graphic/texture.hpp>

namespace Terreate::Graphic::Compute {
using namespace Terreate::Types;
using namespace Terreate::Math;

class ComputeKernel final {
private:
  bool mCompiled = false;
  bool mLinked = false;
  Core::Object mKernelID = Core::Object();
  Str mKernelSource = "";

public:
  /*
   * @brief: OpenGL compute shader wrapper class.
   */
  ComputeKernel();
  ~ComputeKernel();

  u32 getID() const { return mKernelID; }

  /*
   * @brief: Get attribute index
   * @param: name: name of attribute
   * @return: attribute index
   */
  unsigned getAttribute(Str const &name) const {
    return glGetAttribLocation(mKernelID, name.c_str());
  }
  /*
   * @brief: Getter for shader uniform ID.
   * @param: name: name of uniform
   * @return: uniform ID
   */
  unsigned getLocation(Str const &name) const {
    return glGetUniformLocation(mKernelID, name.c_str());
  }
  /*
   * @brief: Getter for shader storage block index.
   * @param: name: name of storage block
   * @return: storage block index
   */
  unsigned getStorageBlockIndex(Str const &name) const {
    return glGetProgramResourceIndex(mKernelID, GL_SHADER_STORAGE_BLOCK,
                                     name.c_str());
  }

  /*
   * @brief: Setter for shader Bool uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, bool const &value) const;
  /*
   * @brief: Setter for shader Bool array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void setUniform(Str const &name, bool const *value, u32 const &count) const;
  /*
   * @brief: Setter for shader Int uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, i32 const &value) const;
  /*
   * @brief: Setter for shader Int array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void setUniform(Str const &name, i32 const *value, u32 const &count) const;
  /*
   * @brief: Setter for shader Int array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, Vec<i32> value) const;
  /*
   * @brief: Setter for shader float uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, float const &value) const;
  /*
   * @brief: Setter for shader float array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   * @param: count: number of elements in array
   */
  void setUniform(Str const &name, float const *value, u32 const &count) const;
  /*
   * @brief: Setter for shader float array uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, Vec<float> value) const;
  /*
   * @brief: Setter for shader vec2 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, vec2 const &value) const;
  /*
   * @brief: Setter for shader vec3 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, vec3 const &value) const;
  /*
   * @brief: Setter for shader vec4 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, vec4 const &value) const;
  /*
   * @brief: Setter for shader mat2 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, mat2 const &value) const;
  /*
   * @brief: Setter for shader mat3 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, mat3 const &value) const;
  /*
   * @brief: Setter for shader mat4 uniform.
   * @param: name: name of uniform
   * @param: value: value of uniform
   */
  void setUniform(Str const &name, mat4 const &value) const;

  /*
   * @brief: Add kernel source.
   * @param: source: source code to add
   */
  void addKernelSource(Str const &source) { mKernelSource += source; }

  /*
   * @brief: This function binds storage block index to binding point.
   * @param: name: name of storage block
   * @param: binding: binding point
   */
  void bindStorageBlock(Str const &name, u32 const &binding) const {
    glShaderStorageBlockBinding(mKernelID, this->getStorageBlockIndex(name),
                                binding);
  }
  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: texture: texture to bind
   */
  void bindImage(Str const &name, u32 const &texture) const;
  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: texture: texture to bind
   */
  void bindImage(Str const &name, Texture const &texture) const;
  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: texture: texture to bind
   */
  void bindReadImage(Str const &name, u32 const &texture) const;
  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: texture: texture to bind
   */
  void bindReadImage(Str const &name, Texture const &texture) const;
  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: texture: texture to bind
   */
  void bindWriteImage(Str const &name, u32 const &texture) const;
  /*
   * @brief: This function binds uniform block index to binding point.
   * @param: name: name of uniform block
   * @param: texture: texture to bind
   */
  void bindWriteImage(Str const &name, Texture const &texture) const;

  /*
   * @brief: Compile shader.
   */
  void compile();
  /*
   * @brief: Link shader.
   */
  void link();
  /*
   * @brief: This function activates selected texture binding point.
   * @param: target: texture binding point
   */
  void activateTexture(TextureTargets const &target) const;
  /*
   * @brief: Dispatch compute shader.
   * @param: x: number of work groups in x direction
   */
  void dispatch(u32 const &x) const { this->dispatch(x, 1, 1); }
  /*
   * @brief: Dispatch compute shader.
   * @param: x: number of work groups in x direction
   * @param: y: number of work groups in y direction
   */
  void dispatch(u32 const &x, u32 const &y) const { this->dispatch(x, y, 1); }
  /*
   * @brief: Dispatch compute shader.
   * @param: x: number of work groups in x direction
   * @param: y: number of work groups in y direction
   * @param: z: number of work groups in z direction
   */
  void dispatch(u32 const &x, u32 const &y, u32 const &z) const;

  operator bool() const { return mKernelID; }
};
} // namespace Terreate::Graphic::Compute
#endif // __TERREATE_GRAPHICS_KERNEL_HPP__
