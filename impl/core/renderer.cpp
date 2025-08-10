#include <core/exception.hpp>
#include <core/renderer.hpp>

namespace Terreate::Core {
Renderer::Renderer() {
  mShader = std::make_shared<OpenGL::Shader>();
  if (!mShader) {
    throw Exception::RendererError("Failed to create OpenGL shader.");
  }
}

void Renderer::attach(shared<Resource::Shader> shader) {
  switch (shader->getType()) {
  case Resource::ShaderType::FRAGMENT:
    mShader->addFragmentShaderSource(shader->getSource());
    break;
  case Resource::ShaderType::VERTEX:
    mShader->addVertexShaderSource(shader->getSource());
    break;
  default:
    // Handle other shader types if necessary
    break;
  }
}

void Renderer::bundle() {
  mShader->compile();
  mShader->link();
}
} // namespace Terreate::Core
