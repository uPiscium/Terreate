#include "../../../include/module/common/exception.hpp"
#include "../../../include/module/opengl/compute.hpp"
#include "../../../include/module/opengl/shader.hpp"

namespace Terreate::OpenGL {
Shader::Shader() {
  mShaderID = glCreateProgram();
  mCompiled = false;
}

Shader::~Shader() {
  if (mShaderID.Count() <= 1) {
    glDeleteProgram(mShaderID);
    mShaderID.Delete();
  }
}

void Shader::setBlending(BlendingFunction const &src,
                         BlendingFunction const &dst) {
  mOption.src = src;
  mOption.dst = dst;
}

void Shader::setCullingFace(CullingFace const &face,
                            CullingMode const &frontFace) {
  mOption.cullFace = face;
  mOption.frontFace = frontFace;
}

void Shader::setStencilFunction(StencilFunction const &func, int const &ref,
                                u32 const &mask) {
  mOption.stencilFunc = func;
  mOption.stencilRef = ref;
  mOption.stencilMask = mask;
}

void Shader::setStencilOperation(StencilOperation const &sFail,
                                 StencilOperation const &dpFail,
                                 StencilOperation const &dpPass) {
  mOption.sFail = sFail;
  mOption.dpFail = dpFail;
  mOption.dpPass = dpPass;
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

  u32 vertID = 0;
  vertID = glCreateShader(GL_VERTEX_SHADER);
  char const *vertSource = mVertexShaderSource.c_str();
  glShaderSource(vertID, 1, &vertSource, NULL);
  glCompileShader(vertID);
  checkCompileStatus(vertID, "Vertex");

  u32 fragID = 0;
  fragID = glCreateShader(GL_FRAGMENT_SHADER);
  char const *fragSource = mFragmentShaderSource.c_str();
  glShaderSource(fragID, 1, &fragSource, NULL);
  glCompileShader(fragID);
  checkCompileStatus(fragID, "Fragment");

  glAttachShader(mShaderID, vertID);
  glAttachShader(mShaderID, fragID);

  if (mGeometryShaderSource != "") {
    u32 geomID = 0;
    geomID = glCreateShader(GL_GEOMETRY_SHADER);
    char const *geomSource = mGeometryShaderSource.c_str();
    glShaderSource(geomID, 1, &geomSource, NULL);
    glCompileShader(geomID);
    checkCompileStatus(geomID, "Geometry");

    glAttachShader(mShaderID, geomID);
  }
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

void Shader::use() const {
  if (!mCompiled) {
    throw Exception::ShaderError("Shader is not compiled");
    return;
  }

  glUseProgram(mShaderID);

  if (mOption.blending) {
    glEnable(GL_BLEND);
    glBlendFunc((GLenum)mOption.src, (GLenum)mOption.dst);
  } else {
    glDisable(GL_BLEND);
  }

  if (mOption.depth) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc((GLenum)mOption.depthFunc);
  } else {
    glDisable(GL_DEPTH_TEST);
  }

  if (mOption.stencil) {
    glEnable(GL_STENCIL_TEST);
    glStencilFunc((GLenum)mOption.stencilFunc, mOption.stencilRef,
                  mOption.stencilMask);
    glStencilOp((GLenum)mOption.sFail, (GLenum)mOption.dpFail,
                (GLenum)mOption.dpPass);
  } else {
    glDisable(GL_STENCIL_TEST);
  }

  if (mOption.scissor) {
    glEnable(GL_SCISSOR_TEST);
  } else {
    glDisable(GL_SCISSOR_TEST);
  }

  if (mOption.culling) {
    glEnable(GL_CULL_FACE);
    glCullFace((GLenum)mOption.cullFace);
    glFrontFace((GLenum)mOption.frontFace);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

} // namespace Terreate::OpenGL
