#ifndef __TERREATE_RENDERER_HPP__
#define __TERREATE_RENDERER_HPP__

#include <bindable.hpp>
#include <core/math.hpp>
#include <core/property.hpp>
#include <graphic/shader.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;
using namespace Terreate::Math;

typedef Graphic::BlendingFunction BlendingFunction;
typedef Graphic::CullingFace CullingFace;
typedef Graphic::CullingFrontFace CullingFrontFace;
typedef Graphic::DepthFunction DepthFunction;
typedef Graphic::StencilFunction StencilFunction;
typedef Graphic::StencilOperation StencilOperation;
typedef Graphic::ShaderOption ShaderOption;

class Renderer : public Interface::IBindable {
private:
  Graphic::Shader mShader;
  ShaderOption mOption;

private:
  void InitProperty();

public:
  Core::Property<Bool> blending;
  Core::Property<Bool> culling;
  Core::Property<Bool> depth;
  Core::Property<Bool> scissor;
  Core::Property<Bool> stencil;
  Core::Property<BlendingFunction> sourceBlending;
  Core::Property<BlendingFunction> destinationBlending;
  Core::Property<CullingFace> cullingFace;
  Core::Property<CullingFrontFace> cullingFrontFace;
  Core::Property<DepthFunction> depthFunction;
  Core::Property<StencilFunction> stencilFunction;
  Core::Property<Int> stencilReference;
  Core::Property<Uint> stencilMask;
  Core::Property<StencilOperation> stencilFail;
  Core::Property<StencilOperation> stencilPass;
  Core::Property<StencilOperation> stencilZFail;

public:
  Renderer(Str const &vertexShader, Str const &fragmentShader);
  Renderer(Str const &vertexShader, Str const &fragmentShader,
           ShaderOption const &option);
  Renderer(Str const &vertexShader, Str const &fragmentShader,
           Str const &geometryShader);
  Renderer(Str const &vertexShader, Str const &fragmentShader,
           Str const &geometryShader, ShaderOption const &option);
  virtual ~Renderer() = default;
  virtual void SetUniform(Str const &name, Bool const &value) {
    mShader.SetBool(name, value);
  }
  virtual void SetUniform(Str const &name, Int const &value) {
    mShader.SetInt(name, value);
  }
  virtual void SetUniform(Str const &name, Int const *value,
                          Uint const &count) {
    mShader.SetInts(name, value, count);
  }
  virtual void SetUniform(Str const &name, Vec<Int> const &value) {
    this->SetUniform(name, value.data(), value.size());
  }
  virtual void SetUniform(Str const &name, Float const &value) {
    mShader.SetFloat(name, value);
  }
  virtual void SetUniform(Str const &name, Float const *value,
                          Uint const &count) {
    mShader.SetFloats(name, value, count);
  }
  virtual void SetUniform(Str const &name, Vec<Float> const &value) {
    this->SetUniform(name, value.data(), value.size());
  }
  virtual void SetUniform(Str const &name, vec2 const &value) {
    mShader.SetVec2(name, value);
  }
  virtual void SetUniform(Str const &name, vec3 const &value) {
    mShader.SetVec3(name, value);
  }
  virtual void SetUniform(Str const &name, vec4 const &value) {
    mShader.SetVec4(name, value);
  }
  virtual void SetUniform(Str const &name, mat2 const &value) {
    mShader.SetMat2(name, value);
  }
  virtual void SetUniform(Str const &name, mat3 const &value) {
    mShader.SetMat3(name, value);
  }
  virtual void SetUniform(Str const &name, mat4 const &value) {
    mShader.SetMat4(name, value);
  }
  virtual void LoadShaderOption(ShaderOption const &option);

  void Bind() const override { mShader.Bind(); }
  void Unbind() const override { mShader.Unbind(); }
};
} // namespace Terreate

#endif // __TERREATE_RENDERER_HPP__
