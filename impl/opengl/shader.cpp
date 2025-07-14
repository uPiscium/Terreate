#include <opengl/compute.hpp>
#include <opengl/exception.hpp>
#include <opengl/shader.hpp>

namespace Terreate::OpenGL {
void Blending::apply() const {
  if (this->enable) {
    glEnable(GL_BLEND);
    glBlendFunc((u32)this->src, (u32)this->dst);
  } else {
    glDisable(GL_BLEND);
  }
}

void Culling::apply() const {
  if (this->enable) {
    glEnable(GL_CULL_FACE);
    glCullFace((u32)this->face);
    glFrontFace((u32)this->frontFace);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

void Depth::apply() const {
  if (this->enable) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc((u32)this->func);
    glDepthMask(this->writable);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void Scissor::apply() const {
  if (this->enable) {
    glEnable(GL_SCISSOR_TEST);
    glScissor(this->x, this->y, this->width, this->height);
  } else {
    glDisable(GL_SCISSOR_TEST);
  }
}

void Stencil::apply() const {
  if (this->enable) {
    glEnable(GL_STENCIL_TEST);
    glStencilFunc((u32)this->func, this->ref, this->mask);
    glStencilOp((u32)this->sFail, (u32)this->dpFail, (u32)this->dpPass);
  } else {
    glDisable(GL_STENCIL_TEST);
  }
}

Shader::Shader() {
  mShaderID = glCreateProgram();
  mCompiled = false;
}

Shader::~Shader() {
  if (mShaderID != 0) {
    glDeleteProgram(mShaderID);
    mShaderID = 0;
  }
}

void Shader::compile() {
  if (mVertexShaderSource == "") {
    throw Exception::ShaderError("Vertex shader source is empty");
    return;
  }

  if (mFragmentShaderSource == "") {
    throw Exception::ShaderError("Fragment shader source is empty");
    return;
  }

  GLObject vertID = 0;
  vertID = glCreateShader(GL_VERTEX_SHADER);
  char const *vertSource = mVertexShaderSource.c_str();
  glShaderSource(vertID, 1, &vertSource, NULL);
  glCompileShader(vertID);
  checkCompileStatus(vertID, "Vertex");

  GLObject fragID = 0;
  fragID = glCreateShader(GL_FRAGMENT_SHADER);
  char const *fragSource = mFragmentShaderSource.c_str();
  glShaderSource(fragID, 1, &fragSource, NULL);
  glCompileShader(fragID);
  checkCompileStatus(fragID, "Fragment");

  glAttachShader(mShaderID, vertID);
  glAttachShader(mShaderID, fragID);

  mCompiled = true;
}

void Shader::link() {
  if (!mCompiled) {
    throw Exception::ShaderError("Shader is not compiled");
    return;
  }

  glLinkProgram(mShaderID);
  checkLinkStatus(mShaderID);
  mLinked = true;
}

str Shader::loadShaderSource(str const &path) {
  ifstream file;
  file.open(path.c_str());

  if (!file.is_open()) {
    throw Exception::ShaderError("Failed to open file: " + path);
    return "";
  }

  std::stringstream stream;
  stream << file.rdbuf();
  file.close();

  return stream.str();
}

void Shader::bind() const {
  if (!mCompiled) {
    throw Exception::ShaderError("Shader is not compiled");
    return;
  }

  glUseProgram(mShaderID);

  mProperties.blending.apply();
  mProperties.culling.apply();
  mProperties.depth.apply();
  mProperties.scissor.apply();
  mProperties.stencil.apply();
}

} // namespace Terreate::OpenGL
