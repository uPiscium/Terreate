#include <renderer.hpp>

namespace Terreate {
void Renderer::InitProperty() {
  this->blending.setter = [this](Bool const &value) {
    mOption.blending = value;
    mShader.SetOption(mOption);
  };
  this->culling.setter = [this](Bool const &value) {
    mOption.culling = value;
    mShader.SetOption(mOption);
  };
  this->depth.setter = [this](Bool const &value) {
    mOption.depth = value;
    mShader.SetOption(mOption);
  };
  this->scissor.setter = [this](Bool const &value) {
    mOption.scissor = value;
    mShader.SetOption(mOption);
  };
  this->stencil.setter = [this](Bool const &value) {
    mOption.stencil = value;
    mShader.SetOption(mOption);
  };
  this->sourceBlending.setter = [this](BlendingFunction const &value) {
    mOption.src = value;
    mShader.SetOption(mOption);
  };
  this->destinationBlending.setter = [this](BlendingFunction const &value) {
    mOption.dst = value;
    mShader.SetOption(mOption);
  };
  this->cullingFace.setter = [this](CullingFace const &value) {
    mOption.cullFace = value;
    mShader.SetOption(mOption);
  };
  this->cullingFrontFace.setter = [this](CullingFrontFace const &value) {
    mOption.frontFace = value;
    mShader.SetOption(mOption);
  };
  this->depthFunction.setter = [this](DepthFunction const &value) {
    mOption.depthFunc = value;
    mShader.SetOption(mOption);
  };
  this->stencilFunction.setter = [this](StencilFunction const &value) {
    mOption.stencilFunc = value;
    mShader.SetOption(mOption);
  };
  this->stencilReference.setter = [this](Int const &value) {
    mOption.stencilRef = value;
    mShader.SetOption(mOption);
  };
  this->stencilMask.setter = [this](Uint const &value) {
    mOption.stencilMask = value;
    mShader.SetOption(mOption);
  };
  this->stencilFail.setter = [this](StencilOperation const &value) {
    mOption.sFail = value;
    mShader.SetOption(mOption);
  };
  this->stencilPass.setter = [this](StencilOperation const &value) {
    mOption.dpFail = value;
    mShader.SetOption(mOption);
  };
  this->stencilZFail.setter = [this](StencilOperation const &value) {
    mOption.dpPass = value;
    mShader.SetOption(mOption);
  };
}

Renderer::Renderer(Str const &vertexShader, Str const &fragmentShader) {
  mShader = Graphic::Shader();
  mShader.Compile(vertexShader, fragmentShader);
  mShader.Link();
  mOption = ShaderOption();
  mShader.SetOption(mOption);
  this->InitProperty();
}

Renderer::Renderer(Str const &vertexShader, Str const &fragmentShader,
                   ShaderOption const &option) {
  mShader = Graphic::Shader();
  mShader.Compile(vertexShader, fragmentShader);
  mShader.Link();
  mOption = option;
  mShader.SetOption(mOption);
  this->InitProperty();
}

Renderer::Renderer(Str const &vertexShader, Str const &fragmentShader,
                   Str const &geometryShader) {
  mShader = Graphic::Shader();
  mShader.Compile(vertexShader, fragmentShader, geometryShader);
  mShader.Link();
  mOption = ShaderOption();
  mShader.SetOption(mOption);
  this->InitProperty();
}

Renderer::Renderer(Str const &vertexShader, Str const &fragmentShader,
                   Str const &geometryShader, ShaderOption const &option) {
  mShader = Graphic::Shader();
  mShader.Compile(vertexShader, fragmentShader, geometryShader);
  mShader.Link();
  mOption = option;
  mShader.SetOption(mOption);
  this->InitProperty();
}

void Renderer::LoadShaderOption(ShaderOption const &option) {
  mOption = option;
  mShader.SetOption(mOption);
  this->InitProperty();
}

} // namespace Terreate
