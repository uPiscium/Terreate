#include <opengl/buffer.hpp>

namespace Terreate::OpenGL {
Buffer::~Buffer() {
  if (mVAO != 0) {
    glDeleteVertexArrays(1, &mVAO);
    mVAO = 0;
  }
  if (mIBO != 0) {
    glDeleteBuffers(1, &mIBO);
    mIBO = 0;
  }
  for (auto &buffer : mBuffers) {
    if (buffer != 0) {
      glDeleteBuffers(1, &buffer);
    }
  }
  mBuffers.clear();
}

void Buffer::setAttributeDivisor(str const &attrName,
                                 u32 const &divisor) const {
  this->bind();
  glVertexAttribDivisor(GLSL_ATTRIBUTE_LOCATIONS.at(attrName), divisor);
  this->unbind();
}

void Buffer::loadData(vec<float> const &raw, BufferUsage const &usage) {
  u64 size = raw.size() * sizeof(float);
  this->bind();
  GLObject buffer = 0;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, raw.data(), (GLenum)usage);

  for (auto &[name, attr] : GLSL_INPUT_ATTRIBUTES) {
    if (!GLSL_ATTRIBUTE_LOCATIONS.contains(
            name)) { // This check should always pass
      throw Exception::BufferError(
          "Attribute location not found. This is a "
          "critical bug. Please report as 'ATTRIBUTE_LOCATION_MISMATCH'.");
    }
    u32 index = GLSL_ATTRIBUTE_LOCATIONS.at(name);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, attr.size, GL_FLOAT, GL_FALSE, attr.stride,
                          reinterpret_cast<void const *>(attr.offset));
    mBufferAttributeMap.insert({name, mBuffers.size()});
  }
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  mBuffers.push_back(buffer);
}

void Buffer::reloadData(vec<float> const &raw) {
  u64 size = raw.size() * sizeof(float);
  this->bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[0]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, raw.data());
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::reloadDataTo(vec<float> const &raw, u32 const &index) {
  if (index >= mBuffers.size()) {
    throw Exception::BufferError("Index out of bounds for buffer.");
  }
  u64 size = raw.size() * sizeof(float);
  this->bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[index]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, raw.data());
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::loadIndices(vec<u32> const &indices) {
  mLoadedIndices = true;
  mIndexCount = indices.size();
  this->bind();
  glGenBuffers(1, &mIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32),
               indices.data(), GL_STATIC_DRAW);
  this->unbind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Buffer::reloadIndices(vec<u32> const &indices) {
  mIndexCount = indices.size();
  this->bind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(u32),
                  indices.data());
  this->unbind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Buffer::draw(DrawMode const &mode) const {
  if (!mLoadedIndices) {
    throw Exception::BufferError("No indices loaded into buffer.");
  }

  if (mBuffers.size() == 0) {
    throw Exception::BufferError("No buffers attached to buffer.");
  }

  this->bind();
  glDrawElements((GLenum)mode, mIndexCount, GL_UNSIGNED_INT, 0);
  this->unbind();
}

void Buffer::draw(DrawMode const &mode, u64 const &count) const {
  if (!mLoadedIndices) {
    throw Exception::BufferError("No indices loaded into buffer.");
  }

  if (mBuffers.size() == 0) {
    throw Exception::BufferError("No buffers attached to buffer.");
  }

  this->bind();
  glDrawElementsInstanced((GLenum)mode, mIndexCount, GL_UNSIGNED_INT, nullptr,
                          count);
  this->unbind();
}

UniformBuffer::~UniformBuffer() {
  if (mUBO != 0) {
    glDeleteBuffers(1, &mUBO);
    mUBO = 0;
  }
}

void UniformBuffer::allocate(u64 const &size, BufferUsage const &usage) {
  this->bind();
  glBufferData(GL_UNIFORM_BUFFER, size, nullptr, (GLenum)usage);
  this->unbind();
}

void UniformBuffer::bind(Shader const &shader, str const &name) const {
  shader.bindUniformBlock(name, (u32)mUBO);
  glBindBufferBase(GL_UNIFORM_BUFFER, (u32)mUBO, mUBO);
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
  if (mSSBO != 0) {
    glDeleteBuffers(1, &mSSBO);
    mSSBO = 0;
  }
}

void ShaderStorageBuffer::allocate(u64 const &size, BufferUsage const &usage) {
  this->bind();
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, (GLenum)usage);
  mSize = size;
  this->unbind();
}

void ShaderStorageBuffer::bind(Shader &shader, str const &name) const {
  shader.bindStorageBlock(name, (u32)mSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (u32)mSSBO, mSSBO);
}

void ShaderStorageBuffer::bind(ComputeKernel &shader, str const &name) const {
  shader.bindStorageBlock(name, (u32)mSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (u32)mSSBO, mSSBO);
}
} // namespace Terreate::OpenGL
