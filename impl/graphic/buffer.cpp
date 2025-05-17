#include <graphic/buffer.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

Vec<float> const &BufferDataConstructor::getVertexData() const {
  if (!mConstructed) {
    throw Exceptions::BufferError("Data not constructed.");
  }
  return mVertexData;
}

void BufferDataConstructor::addVertexComponent(Str const &name,
                                               Vec<Vec<float>> const &data) {
  mUpdated = true;
  mVertexDataComponents.push_back(data);
  mAttributeNames.push_back(name);
  mAttributes.insert(
      {name,
       {0, mVertexDataComponents.size() - 1, data[0].size(), 0, mOffset}});
  mOffset += data[0].size() * sizeof(float);
}

void BufferDataConstructor::reloadVertexComponent(Str const &name,
                                                  Vec<Vec<float>> const &data) {
  mUpdated = true;
  if (mAttributes.find(name) == mAttributes.end()) {
    this->addVertexComponent(name, data);
    return;
  }

  u32 index = mAttributes.at(name).index;
  mVertexDataComponents[index] = data;
}

void BufferDataConstructor::construct() {
  if (mVertexDataComponents.size() == 0) {
    throw Exceptions::BufferError("No data components added.");
  }

  if (mVertexIndices.size() == 0) {
    throw Exceptions::BufferError("No indices added.");
  }

  if (mVertexIndices[0].size() != mVertexDataComponents.size()) {
    throw Exceptions::BufferError("Indices and data components mismatch.");
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
  if (mVAO.count() <= 1) {
    glDeleteVertexArrays(1, mVAO);
    mVAO.dispose();
  }
  if (mIBO.count() <= 1) {
    glDeleteBuffers(1, mIBO);
    mIBO.dispose();
  }
  for (auto &buffer : mBuffers) {
    if (buffer.count() <= 1) {
      glDeleteBuffers(1, buffer);
      buffer.dispose();
    }
  }
}

void Buffer::setAttributeDivisor(AttributeData const &attribute,
                                 u32 const &divisor) const {
  this->bind();
  glVertexAttribDivisor(attribute.index, divisor);
  this->unbind();
}

void Buffer::loadData(Vec<float> const &raw,
                      Map<Str, AttributeData> const &attrs,
                      Map<Str, u32> const &locations,
                      BufferUsage const &usage) {
  u64 size = raw.size() * sizeof(float);
  this->bind();
  Core::Object buffer = Core::Object();
  glGenBuffers(1, buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, raw.data(), (GLenum)usage);

  for (auto &[name, attr] : attrs) {
    if (locations.find(name) == locations.end()) {
      throw Exceptions::BufferError("Attribute location not found.");
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
                      Map<Str, u32> const &locations,
                      BufferUsage const &usage) {
  Map<Str, AttributeData> const &attributes = bdc.getAttributes();
  Vec<float> data = bdc.getVertexData();
  u64 size = data.size() * sizeof(float);
  this->bind();
  Core::Object buffer = Core::Object();
  glGenBuffers(1, buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, size, data.data(), (GLenum)usage);

  for (auto &name : bdc.getAttributeNames()) {
    if (locations.find(name) == locations.end()) {
      throw Exceptions::BufferError("Attribute location not found.");
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
  Vec<float> data = bdc.getVertexData();
  u64 size = data.size() * sizeof(float);
  this->bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[target.vboIndex]);
  glBufferSubData(GL_ARRAY_BUFFER, target.offset, size, data.data());
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::reloadData(AttributeData const &target,
                        BufferDataConstructor const &bdc, u32 const &offset) {
  Vec<float> data = bdc.getVertexData();
  u64 size = data.size() * sizeof(float);
  this->bind();
  glBindBuffer(GL_ARRAY_BUFFER, mBuffers[target.vboIndex]);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data.data());
  this->unbind();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::loadIndices(Vec<u32> const &indices) {
  mLoadedIndices = true;
  mIndexCount = indices.size();
  this->bind();
  glGenBuffers(1, mIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32),
               indices.data(), GL_STATIC_DRAW);
  this->unbind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Buffer::reloadIndices(Vec<u32> const &indices) {
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
    throw Exceptions::BufferError("No indices loaded into buffer.");
  }

  if (mBuffers.size() == 0) {
    throw Exceptions::BufferError("No buffers attached to buffer.");
  }

  this->bind();
  glDrawElements((GLenum)mode, mIndexCount, GL_UNSIGNED_INT, 0);
  this->unbind();
}

void Buffer::draw(DrawMode const &mode, u64 const &count) const {
  if (!mLoadedIndices) {
    throw Exceptions::BufferError("No indices loaded into buffer.");
  }

  if (mBuffers.size() == 0) {
    throw Exceptions::BufferError("No buffers attached to buffer.");
  }

  this->bind();
  glDrawElementsInstanced((GLenum)mode, mIndexCount, GL_UNSIGNED_INT, nullptr,
                          count);
  this->unbind();
}

UniformBuffer::~UniformBuffer() {
  if (mUBO.count() <= 1) {
    glDeleteBuffers(1, mUBO);
    mUBO.dispose();
  }
}

void UniformBuffer::allocate(u64 const &size, BufferUsage const &usage) {
  this->bind();
  glBufferData(GL_UNIFORM_BUFFER, size, nullptr, (GLenum)usage);
  this->unbind();
}

void UniformBuffer::bind(Shader const &shader, Str const &name) const {
  shader.bindUniformBlock(name, (u32)mUBO);
  glBindBufferBase(GL_UNIFORM_BUFFER, (u32)mUBO, mUBO);
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
  if (mSSBO.count() <= 1) {
    glDeleteBuffers(1, mSSBO);
    mSSBO.dispose();
  }
}

void ShaderStorageBuffer::allocate(u64 const &size, BufferUsage const &usage) {
  this->bind();
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, (GLenum)usage);
  mSize = size;
  this->unbind();
}

void ShaderStorageBuffer::bind(Shader &shader, Str const &name) const {
  shader.bindStorageBlock(name, (u32)mSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (u32)mSSBO, mSSBO);
}

void ShaderStorageBuffer::bind(Compute::ComputeKernel &shader,
                               Str const &name) const {
  shader.bindStorageBlock(name, (u32)mSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (u32)mSSBO, mSSBO);
}
} // namespace Terreate::Graphic
