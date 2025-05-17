#include <core/exception.hpp>
#include <graphic/shader.hpp>

namespace Terreate::Graphic {
Str getShaderLog(u32 const &id) {
  i32 status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status) {
    return "";
  }

  char log[1024];
  glGetShaderInfoLog(id, 1024, NULL, log);
  return log;
}

void checkCompileStatus(u32 const &id, Str const &name) {
  Str log = getShaderLog(id);
  if (log != "") {
    log = name + " shader log: " + log;
    throw Exceptions::ShaderError(log);
  }
}

Str getProgramLog(u32 const &id) {
  i32 status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);

  if (status) {
    return "";
  }

  char log[1024];
  glGetProgramInfoLog(id, 1024, NULL, log);
  return log;
}

void checkLinkStatus(u32 const &id) {
  Str log = getProgramLog(id);

  if (log != "") {
    log = "Shader program log: " + log;
    throw Exceptions::ShaderError(log);
  }
}

Shader::Shader() {
  mShaderID = glCreateProgram();
  mCompiled = false;
}

Shader::~Shader() {
  if (mShaderID.count() <= 1) {
    glDeleteProgram(mShaderID);
    mShaderID.dispose();
  }
}

void Shader::compile(Str const &vertex, Str const &fragment,
                     Str const &geometry) {
  Str vsrc = "";
  Str fsrc = "";
  if (vertex == "") {
    vsrc = VERTEX_SHADER_SOURCE_DEFAULT;
    return;
  }

  if (fragment == "") {
    fsrc = FRAGMENT_SHADER_SOURCE_DEFAULT;
    return;
  }

  ID vertID = 0;
  vertID = glCreateShader(GL_VERTEX_SHADER);
  char const *vertSource = vsrc.c_str();
  glShaderSource(vertID, 1, &vertSource, NULL);
  glCompileShader(vertID);
  checkCompileStatus(vertID, "Vertex");

  ID fragID = 0;
  fragID = glCreateShader(GL_FRAGMENT_SHADER);
  char const *fragSource = fsrc.c_str();
  glShaderSource(fragID, 1, &fragSource, NULL);
  glCompileShader(fragID);
  checkCompileStatus(fragID, "Fragment");

  glAttachShader(mShaderID, vertID);
  glAttachShader(mShaderID, fragID);

  if (geometry != "") {
    ID geomID = 0;
    geomID = glCreateShader(GL_GEOMETRY_SHADER);
    char const *geomSource = geometry.c_str();
    glShaderSource(geomID, 1, &geomSource, NULL);
    glCompileShader(geomID);
    checkCompileStatus(geomID, "Geometry");

    glAttachShader(mShaderID, geomID);
  }
  mCompiled = true;
}

void Shader::link() {
  if (!mCompiled) {
    throw Exceptions::ShaderError("Shader is not compiled");
    return;
  }

  glLinkProgram(mShaderID);
  checkLinkStatus(mShaderID);
  mLinked = true;
}

Str Shader::loadShaderSource(const Str &path) {
  InputFileStream file;
  file.open(path.c_str());

  if (!file.is_open()) {
    throw Exceptions::ShaderError("Failed to open file: " + path);
    return "";
  }

  std::stringstream stream;
  stream << file.rdbuf();
  file.close();

  return stream.str();
}

void Shader::bind() const {
  if (!mCompiled) {
    throw Exceptions::ShaderError("Shader is not compiled");
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

} // namespace Terreate::Graphic
