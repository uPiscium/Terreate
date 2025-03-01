#include <exceptions.hpp>
#include <graphic/shader.hpp>

namespace Terreate::Graphic {
Str GetShaderLog(Uint const &id) {
  Int status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status) {
    return "";
  }

  char log[1024];
  glGetShaderInfoLog(id, 1024, NULL, log);
  return log;
}

void CheckCompileStatus(Uint const &id, Str const &name) {
  Str log = GetShaderLog(id);
  if (log != "") {
    log = name + " shader log: " + log;
    throw Exceptions::ShaderError(log);
  }
}

Str GetProgramLog(Uint const &id) {
  Int status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);

  if (status) {
    return "";
  }

  char log[1024];
  glGetProgramInfoLog(id, 1024, NULL, log);
  return log;
}

void CheckLinkStatus(Uint const &id) {
  Str log = GetProgramLog(id);

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
  if (mShaderID.Count() <= 1) {
    glDeleteProgram(mShaderID);
    mShaderID.Delete();
  }
}

void Shader::SetBlending(BlendingFunction const &src,
                         BlendingFunction const &dst) {
  mOption.src = src;
  mOption.dst = dst;
}

void Shader::SetCullingFace(CullingFace const &face,
                            CullingMode const &frontFace) {
  mOption.cullFace = face;
  mOption.frontFace = frontFace;
}

void Shader::SetStencilFunction(StencilFunction const &func, int const &ref,
                                Uint const &mask) {
  mOption.stencilFunc = func;
  mOption.stencilRef = ref;
  mOption.stencilMask = mask;
}

void Shader::SetStencilOperation(StencilOperation const &sFail,
                                 StencilOperation const &dpFail,
                                 StencilOperation const &dpPass) {
  mOption.sFail = sFail;
  mOption.dpFail = dpFail;
  mOption.dpPass = dpPass;
}

void Shader::Compile() {
  if (mVertexShaderSource == "") {
    throw Exceptions::ShaderError("Vertex shader source is empty");
    return;
  }

  if (mFragmentShaderSource == "") {
    throw Exceptions::ShaderError("Fragment shader source is empty");
    return;
  }

  ID vertID = 0;
  vertID = glCreateShader(GL_VERTEX_SHADER);
  char const *vertSource = mVertexShaderSource.c_str();
  glShaderSource(vertID, 1, &vertSource, NULL);
  glCompileShader(vertID);
  CheckCompileStatus(vertID, "Vertex");

  ID fragID = 0;
  fragID = glCreateShader(GL_FRAGMENT_SHADER);
  char const *fragSource = mFragmentShaderSource.c_str();
  glShaderSource(fragID, 1, &fragSource, NULL);
  glCompileShader(fragID);
  CheckCompileStatus(fragID, "Fragment");

  glAttachShader(mShaderID, vertID);
  glAttachShader(mShaderID, fragID);

  if (mGeometryShaderSource != "") {
    ID geomID = 0;
    geomID = glCreateShader(GL_GEOMETRY_SHADER);
    char const *geomSource = mGeometryShaderSource.c_str();
    glShaderSource(geomID, 1, &geomSource, NULL);
    glCompileShader(geomID);
    CheckCompileStatus(geomID, "Geometry");

    glAttachShader(mShaderID, geomID);
  }
  mCompiled = true;
}

void Shader::Link() {
  if (!mCompiled) {
    throw Exceptions::ShaderError("Shader is not compiled");
    return;
  }

  glLinkProgram(mShaderID);
  CheckLinkStatus(mShaderID);
  mLinked = true;
}

Str Shader::LoadShaderSource(const Str &path) {
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

void Shader::Use() const {
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
