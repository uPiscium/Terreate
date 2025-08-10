#pragma once

#include "../common/math.hpp"
#include "../common/type.hpp"

#include "../opengl/shader.hpp"
#include "../resource/shader.hpp"

namespace Terreate::Core {
class Renderer {
private:
  shared<OpenGL::Shader> mShader;

public:
  Renderer();
  ~Renderer() = default;

  void attach(shared<Resource::Shader> shader);
  void bundle();

  void bind() {
    if (mShader) {
      mShader->bind();
    }
  }
  void unbind() {
    if (mShader) {
      mShader->unbind();
    }
  }
};
} // namespace Terreate::Core
