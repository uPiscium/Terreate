#include <opengl/buffer.hpp>

namespace Terreate::OpenGL {
vec<float> const &BufferDataConstructor::GetVertexData() const {
  if (!mConstructed) {
    throw Exception::BufferError("Data not constructed.");
  }
  return mVertexData;
}

void BufferDataConstructor::AddVertexComponent(str const &name,
                                               vec<vec<float>> const &data) {
  mUpdated = true;
  mVertexDataComponents.push_back(data);
  mAttributeNames.push_back(name);
  mAttributes.insert(
      {name,
       {0, mVertexDataComponents.size() - 1, data[0].size(), 0, mOffset}});
  mOffset += data[0].size() * sizeof(float);
}

void BufferDataConstructor::ReloadVertexComponent(str const &name,
                                                  vec<vec<float>> const &data) {
  mUpdated = true;
  if (mAttributes.find(name) == mAttributes.end()) {
    this->AddVertexComponent(name, data);
    return;
  }

  u32 index = mAttributes.at(name).index;
  mVertexDataComponents[index] = data;
}

void BufferDataConstructor::Construct() {
  if (mVertexDataComponents.size() == 0) {
    throw Exception::BufferError("No data components added.");
  }

  if (mVertexIndices.size() == 0) {
    throw Exception::BufferError("No indices added.");
  }

  if (mVertexIndices[0].size() != mVertexDataComponents.size()) {
    throw Exception::BufferError("Indices and data components mismatch.");
  }

  for (auto &attr : mAttributes) {
    attr.second.stride = mOffset;
  }

  mVertexData.clear();
  for (auto &indices : mVertexIndices) {
    for (u32 i = 0; i < indices.size(); ++i) {
      auto &data = mVertexDataComponents[i];
      u32 &index = indices[i];
      mVertexData.insert(mVertexData.end(), data[index].begin(),
                         data[index].end());
    }
  }
  mConstructed = true;
}

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

void Buffer::setAttributeDivisor(AttributeData const &attribute,
                                 u32 const &divisor) const {
  this->bind();
  glVertexAttribDivisor(attribute.index, divisor);
  this->unbind();
}

void Buffer::loadData(vec<float> const &raw,
                      umap<str, AttributeData> const &attrs,
                      umap<str, u32> const &locations,
                      BufferUsage const &usage) {
  u64 size = raw.size() * sizeof(float);
  this->bind();
  GLObject buffer = 0;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, raw.data(), (GLenum)usage);

  for (auto &[name, attr] : attrs) {
    if (locations.find(name) == locations.end()) {
      throw Exception::BufferError("Attribute location not found.");
    }
    u32 index = locations.at(name);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, attr.size, GL_FLOAT, GL_FALSE, attr.stride,
                          reinterpret_cast<void const *>(attr.offset));
    mAttributes.insert(
        {name, {mBuffers.size(), index, attr.size, attr.stride, attr.offset}});
  }
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  mBuffers.push_back(buffer);
}

void Buffer::loadData(BufferDataConstructor const &bdc,
                      umap<str, u32> const &locations,
                      BufferUsage const &usage) {
  umap<str, AttributeData> const &attributes = bdc.GetAttributes();
  vec<float> data = bdc.GetVertexData();
  u64 size = data.size() * sizeof(float);
  this->bind();
  GLObject buffer = 0;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, data.data(), (GLenum)usage);

  for (auto &name : bdc.GetAttributeNames()) {
    if (locations.find(name) == locations.end()) {
      throw Exception::BufferError("Attribute location not found.");
    }
    AttributeData const &attr = attributes.at(name);
    u32 index = locations.at(name);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, attr.size, GL_FLOAT, GL_FALSE, attr.stride,
                          reinterpret_cast<void const *>(attr.offset));
    mAttributes.insert(
        {name, {mBuffers.size(), index, attr.size, attr.stride, attr.offset}});
  }
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  mBuffers.push_back(buffer);
}

void Buffer::reloadData(AttributeData const &target,
                        BufferDataConstructor const &bdc) {
  vec<float> data = bdc.GetVertexData();
  u64 size = data.size() * sizeof(float);
  this->bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[target.vboIndex]);
  glBufferSubData(GL_ARRAY_BUFFER, target.offset, size, data.data());
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::reloadData(AttributeData const &target,
                        BufferDataConstructor const &bdc, u32 const &offset) {
  vec<float> data = bdc.GetVertexData();
  u64 size = data.size() * sizeof(float);
  this->bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[target.vboIndex]);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data.data());
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
